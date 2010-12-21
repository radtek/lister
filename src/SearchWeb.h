#ifndef _lister_MyTool002_SearchWeb_h_
#define _lister_MyTool002_SearchWeb_h_

//HttpClient::Execute
//	socket host = something:80
//	host = something, port = 80
//	request: GET /something.xml HTTP/1.1
//	URL: http://something/something.xml
//	Host: something
//	Connection: close
//	Accept: */*
//	Agent: Ultimate++ HTTP client
////HttpClient::Execute

#include <Core/Core.h>
#include <Web/Web.h>
using namespace Upp;

/*
#define SPEED_LIMIT 1024 // 1 kB/s

inline double PreciseTime(){
	// returns time with milisecond resolution - there should be something more
	// clever than this, but I was too lazy to search...
	return GetSysTime().Get()+(GetTickCount()%1000)/1000.0;
}

bool Throttler(int written,int length){
	static double start=PreciseTime();
	if(length==0) start=PreciseTime(); // the download goes in chunks, we will count 
	                                   // average speed in each chunk separately (it's easier)
	double elapsed=max(PreciseTime()-start,1e-8); //we don't want to divide by zero!
	double speed=written/elapsed;
	while(speed>SPEED_LIMIT){
		Sleep(10);                   // this loop effectively pauses the download until
		elapsed=PreciseTime()-start; // the average speed doesn't fall back under
		speed=written/elapsed;       // given threshold
	}
	Cout()<<"Time: "<<elapsed<<", "           // This callback is originally meant
	        "Downloaded: "<<written<<         // to be used for reporting the progress
	        " => "<<(speed/1024)<<"kB/s.\n";  // so we do it here too...
	return false; // we always return false, true would cause aborting the download
}

CONSOLE_APP_MAIN{
	HttpClient c("http://www.ultimatepp.org/index.html_0.png");
	String result=c.ExecuteRedirect(5,3,callback(Throttler));
	SaveFile(GetHomeDirFile("upp.png"),result);
}
*/

Hello!

I have applied your modifications to the httpcli.cpp file. The updated uppsrc tree is available on Mirek's UVS repository. I haven't used HEAD myself so far but I believe the code is at least harmless with respect to the existing http client functionality and it sure can be useful in certain situations.

Concerning http client, there are some more directions I would like it to develop. One is that currently it doesn't implement persistent sockets (retrieving multiple requests after a single Connect), there is poor if any support for multiparts and the request execution is monolithic in the sense that you cannot e.g. make a HttpClientStream and incrementally display an image being loaded. Another thing is secure http, i.e. SSL support in the http client. When the time is ready, a potential future revision of the http client should take the above problems into account.

As concerns remote file information retrieval, of course this would be handy in FTP as well. It would be nice to try to tweak the BSD FTP client used (with minor modifications  ) in U++ to do this if possible. This way, it would be even possible to extend the FileSelector to (optionally) support more general URL's like remote FTP or HTTP file repositories.

Regards

Tomas 


#include "Web.h"
#pragma hdrstop

NAMESPACE_UPP

#define LLOG(x)       RLOG(x)
#define LLOGBLOCK(x)  // RLOGBLOCK(x)
#define LDUMP(x)      // RDUMP(x)

HttpClient::HttpClient()
{
	port = DEFAULT_PORT;
	timeout_msecs = DEFAULT_TIMEOUT_MSECS;
	max_header_size = DEFAULT_MAX_HEADER_SIZE;
	max_content_size = DEFAULT_MAX_CONTENT_SIZE;
	keepalive = false;
	std_headers = true;
	method = METHOD_GET;
}

HttpClient& HttpClient::URL(const char *u)
{
	const char *t = u;
	while(*t && *t != '?')
		if(*t++ == '/' && *t == '/') {
			u = ++t;
			break;
		}
	t = u;
	while(*u && *u != ':' && *u != '/' && *u != '?')
		u++;
	host = String(t, u);
	port = DEFAULT_PORT;
	if(*u == ':')
		port = ScanInt(u + 1, &u);
	path = u;
	return *this;
}

HttpClient& HttpClient::Proxy(const char *p)
{
	const char *t = p;
	while(*p && *p != ':')
		p++;
	proxy_host = String(t, p);
	proxy_port = 80;
	if(*p++ == ':' && IsDigit(*p))
		proxy_port = ScanInt(p);
	return *this;
}

String HttpClient::ExecuteRedirect(int max_redirect, int retries, Gate2<int, int> progress)
{
	int nredir = 0;
	for(;;) {
		String data = Execute(progress);
		if(status_code >= 400 && status_code < 500) {
			error = status_line;
			return String::GetVoid();
		}
		int r = 0;
		while(data.IsVoid()) {
			if(++r >= retries)
				return String::GetVoid();
			data = Execute(progress);
		}
		if(!IsRedirect())
			return data;
		if(++nredir > max_redirect) {
			error = NFormat("Maximum number of redirections exceeded: %d", max_redirect);
			return String::GetVoid();
		}
		URL(GetRedirectURL());
	}
}

String HttpClient::ReadUntilProgress(char until, int start_time, int end_time, Gate2<int, int> progress)
{
	String out;
	while(!socket.IsEof() && !socket.IsError()) {
		out.Cat(socket.Read(1000, 1000));
		int f = out.Find('\n');
		if(f >= 0) {
			socket.UnRead(out.Mid(f + 1));
			out.Trim(f);
			return out;
		}
		int t = msecs();
		if(t >= end_time) {
			socket.SetErrorText(NFormat(t_("%s:%d receiving headers timed out"), host, port));
			break;
		}
		if(progress(msecs(start_time), end_time - start_time)) {
			aborted = true;
			break;
		}
	}
	return String::GetVoid();
}

String HttpClient::Execute(Gate2<int, int> progress)
{
	LLOGBLOCK("HttpClient::Execute");
	int start_time = msecs();
	int end_time = start_time + timeout_msecs;
	aborted = false;
	server_headers = Null;
	status_line = Null;
	status_code = 0;
	is_redirect = false;
	redirect_url = Null;
	if(socket.IsOpen() && IsError())
		Close();
	error = Null;
	bool use_proxy = !IsNull(proxy_host);
	String sock_host = (use_proxy ? proxy_host : host);
	int sock_port = (use_proxy ? proxy_port : port);

	LLOG("socket host = " << sock_host << ":" << sock_port);
	if(!socket.IsOpen() && !ClientSocket(socket, sock_host, sock_port, true, NULL, 0, false)) {
		error = Socket::GetErrorText();
		return String::GetVoid();
	}
	while(!socket.PeekWrite(1000)) {
		int time = msecs();
		if(time >= end_time) {
			error = NFormat(t_("%s:%d: connecting to host timed out"), sock_host, sock_port);
			return String::GetVoid();
		}
		if(progress(time - start_time, end_time - start_time)) {
			aborted = true;
			return String::GetVoid();
		}
	}
	String request;
	switch(method) {
		case METHOD_GET:  request << "GET "; break;
		case METHOD_POST: request << "POST "; break;
        case METHOD_HEAD: request << "HEAD "; break;
		default: NEVER(); // invalid method
	}
	String host_port = host;
	if(port != DEFAULT_PORT)
		host_port << ':' << port;
	String url;
	url << "http://" << host_port << Nvl(path, "/");
	if(use_proxy)
		request << url;
	else
		request << Nvl(path, "/");
	request << " HTTP/1.1\r\n";
	if(std_headers) {
		request
			<< "URL: " << url << "\r\n"
			<< "Host: " << host_port << "\r\n"
			<< "Connection: " << (keepalive ? "keep-alive" : "close") << "\r\n";
		if(keepalive)
			request << "Keep-alive: 300\r\n"; // 5 minutes (?)
		request << "Accept: " << Nvl(accept, "*/*") << "\r\n";
		request << "Accept-Encoding: gzip\r\n";
		request << "Agent: " << Nvl(agent, "Ultimate++ HTTP client") << "\r\n";
		if(method == METHOD_POST)
			request << "Content-Length: " << postdata.GetLength() << "\r\n";
	}
	if(!IsNull(username) || !IsNull(password))
		request << "Authorization: basic " << Base64Encode(username + ":" + password) << "\r\n";
	request << client_headers << "\r\n" << postdata;
	LLOG("host = " << host << ", port = " << port);
	LLOG("request: " << request);
	int written = 0;
	while(msecs() < end_time) {
		int nwrite = socket.WriteWait(request.GetIter(written), request.GetLength() - written, 1000);
		if(socket.IsError()) {
			error = Socket::GetErrorText();
			return String::GetVoid();
		}
		if((written += nwrite) >= request.GetLength())
			break;
		if(progress(written, request.GetLength())) {
			aborted = true;
			return String::GetVoid();
		}
	}
	if(written < request.GetLength()) {
		error = NFormat(t_("%s:%d: timed out sending request to server"), host, port);
		return String::GetVoid();
	}
	status_line = ReadUntilProgress('\n', start_time, end_time, progress);
	if(socket.IsError()) {
		error = Socket::GetErrorText();
		return String::GetVoid();
	}
	if(status_line.GetLength() < 5 || MemICmp(status_line, "HTTP/", 5)) {
		error = NFormat(t_("%s:%d: invalid server response: %s"), host, port, status_line);
		return String::GetVoid();
	}

	status_code = 0;
	const char *p = status_line.Begin() + 5;
	while(*p && *p != ' ')
		p++;
	if(*p == ' ' && IsDigit(*++p))
		status_code = stou(p);
	is_redirect = (status_code >= 300 && status_code < 400);

	int content_length = -1;
	bool tc_chunked = false;
	bool ce_gzip = false;
	for(;;) {
		String line = ReadUntilProgress('\n', start_time, end_time, progress);
		if(socket.IsError()) {
			error = Socket::GetErrorText();
			return String::GetVoid();
		}

		for(p = line; *p && (byte)*p <= ' '; p++)
			;
		const char *b = p, *e = line.End();
		while(e > b && (byte)e[-1] < ' ')
			e--;
		if(b >= e)
			break;
		static const char cl[] = "content-length:";
		static const char ce[] = "content-encoding:";
		static const char te[] = "transfer-encoding:";
		static const char lo[] = "location:";
		static const int CL_LENGTH = sizeof(cl) - 1;
		static const int CE_LENGTH = sizeof(ce) - 1;
		static const int TE_LENGTH = sizeof(te) - 1;
		static const int LO_LENGTH = sizeof(lo) - 1;
		if(!MemICmp(p, cl, CL_LENGTH)) {
			for(p += CL_LENGTH; *p == ' '; p++)
				;
			if(IsDigit(*p)) {
				content_length = minmax<int>(stou(p), 0, max_content_size);
				if(content_length > max_content_size) {
					error = NFormat(t_("%s:%d: maximum data length exceeded (%d B)"), host, port, max_content_size);
					return String::GetVoid();
				}
			}
		}
		else if(!MemICmp(p, ce, CE_LENGTH)) {
			for(p += CE_LENGTH; *p == ' '; p++)
				;
			static const char gzip[] = "gzip";
			if(e - p == sizeof(gzip) - 1 && !memcmp(p, gzip, sizeof(gzip) - 1))
				ce_gzip = true;
		}
		else if(!MemICmp(p, te, TE_LENGTH)) {
			for(p += TE_LENGTH; *p == ' '; p++)
				;
			static const char ch[] = "chunked";
			if(e - p == sizeof(ch) - 1 && !memcmp(p, ch, sizeof(ch) - 1))
				tc_chunked = true;
		}
		else if(!MemICmp(p, lo, LO_LENGTH)) {
			for(p += LO_LENGTH; *p == ' '; p++)
				;
			redirect_url = String(p, e);
			int q = redirect_url.Find('?');
			int p = path.Find('?');
			if(p >= 0 && q < 0)
				redirect_url.Cat(path.GetIter(p));
		}
		if(server_headers.GetLength() + (e - b) + 2 > max_header_size) {
			error = NFormat(t_("%s:%d: maximum header length exceeded (%d B)"), host, port, max_header_size);
			return String::GetVoid();
		}
		server_headers.Cat(b, e - b);
		server_headers.Cat("\r\n");
	}
    if (method==METHOD_HEAD)
        return String::GetVoid();
	String chunked;
	String body;
	while(body.GetLength() < content_length || content_length < 0 || tc_chunked) {
		if(msecs(end_time) >= 0) {
			error = NFormat(t_("%s:%d: timed out when receiving server response"), host, port);
			return String::GetVoid();
		}
		String part = socket.Read(1000);
		if(!part.IsEmpty()) {
			if(body.GetLength() + part.GetLength() > DEFAULT_MAX_CONTENT_SIZE) {
				error = NFormat(t_("Maximum content size exceeded: %d"), body.GetLength() + part.GetLength());
				return tc_chunked ? chunked : body;
			}
			body.Cat(part);
			if(tc_chunked)
				for(;;) {
					const char *p = body.Begin(), *e = body.End();
					while(p < e && *p != '\n')
						p++;
					if(p >= e)
						break;
					int nextline = p + 1 - body.Begin();
					p = body.Begin();
					int part_length = ctoi(*p);
					if((unsigned)part_length >= 16) {
						body.Remove(0, nextline);
						continue;
					}
					for(int i; (unsigned)(i = ctoi(*++p)) < 16; part_length = part_length * 16 + i)
						;
					body.Remove(0, nextline);
					if(part_length <= 0) {
						p = body.Begin();
						while(p < e - 2)
							if(*p == '\n' && (p[1] == '\n' || p[1] == '\r' && p[2] == '\n'))
								goto EXIT;
							else
								p++;
						while(socket.IsOpen() && !socket.IsError() && !socket.IsEof()) {
							if(msecs(end_time) >= 0) {
								error = NFormat("Timeout reading footer block (%d B).", body.GetLength());
								goto EXIT;
							}
							if(body.GetLength() > 3)
								body.Remove(0, body.GetLength() - 3);
							String part = socket.Read(1000);
							body.Cat(part);
							const char *p = body;
							while(*p && !(*p == '\n' && (p[1] == '\n' || p[1] == '\r' && p[2] == '\n')))
								p++;
							if(*p)
								goto EXIT;
						}
						break;
					}
					if(body.GetLength() >= part_length) {
						chunked.Cat(body, part_length);
						body.Remove(0, part_length);
						continue;
					}
					for(;;) {
						String part = socket.Read(-msecs(end_time));
						if(part.IsEmpty()) {
							error = NFormat("Timeout reading Transfer-encoding: chunked block (%d B).", part_length);
							goto EXIT;
						}
						body.Cat(part);
						if(body.GetLength() >= part_length) {
							chunked.Cat(body, part_length);
							body.Remove(0, part_length);
							break;
						}
						if(progress(chunked.GetLength() + body.GetLength(), 0)) {
							aborted = true;
							return chunked;
						}
					}
				}
		}
		if(!socket.IsOpen() || socket.IsError() || socket.IsEof()) {
			if(socket.IsError())
				error = Socket::GetErrorText();
			else if(!tc_chunked && content_length > 0 && body.GetLength() < content_length)
				error = NFormat(t_("Partial input: %d out of %d"), body.GetLength(), content_length);
			break;
		}
		if(progress(chunked.GetLength() + body.GetLength(), max(content_length, 0))) {
			aborted = true;
			return tc_chunked ? chunked : body;
		}
	}

EXIT:
	if(!keepalive || socket.IsError() || socket.IsEof())
		Close();

	if(ce_gzip) {
		body = GZDecompress(body);
		return body;
	}
	return tc_chunked ? chunked : body;
}

String HttpClientGet(String url, String proxy, String username, String password,
	String *server_headers, String *error, Gate2<int, int> progress,
	int timeout, int num_redirect, int retries)
{
	HttpClient client;
	String out = client
		.URL(url)
		.User(username, password)
		.TimeoutMsecs(timeout)
		.Proxy(proxy)
		.ExecuteRedirect(num_redirect, progress, progress);
	if(server_headers)
		*server_headers = client.GetHeaders();
	if(error)
		*error = client.GetError();
	return out;
}

String HttpClientGet(String url, String proxy, String *server_headers, String *error,
	Gate2<int, int> progress, int timeout, int max_redirect, int retries)
{
	return HttpClientGet(url, proxy, Null, Null, server_headers, error, progress, timeout, max_redirect, retries);
}

String HttpClientGet(String url, String username, String password,
	String *server_headers, String *error, Gate2<int, int> progress,
	int timeout, int num_redirect, int retries)
{
	return HttpClientGet(url, Null, username, password, server_headers, error, progress, timeout, num_redirect, retries);
}

String HttpClientGet(String url, String *server_headers, String *error,
	Gate2<int, int> progress, int timeout, int max_redirect, int retries)
{
	return HttpClientGet(url, Null, Null, Null, server_headers, error, progress, timeout, max_redirect, retries);
}

END_UPP_NAMESPACE

Hi there!

The main purpose of ExecuteRedirect is to jump over redirection responses. A HTTP server can respond with a result code in the range 300 .. 399 to indicate that the requested resource is available at a different URI. ExecuteRedirect detects these cases and follows the redirection chain to (hopefully) obtain the 'true' final resource. The maximum number of times to hop these redirections is the first parameter in call to ExecuteRedirect and by default is set to the symbolic constant HttpClient::DEFAULT_MAX_REDIRECT ::= 5. Each hop is a separate request and as such it should timeout whenever the server becomes inactive for HttpClient::DEFAULT_TIMEOUT_MSECS ::= 120000 msecs (the default is 2 minutes, see also HttpClient::TimeoutMsecs()).

Note that the timeout check is not entirely reliable: if the server was able to produce, say, 1 byte a minute, the HttpClient would never proclaim it inactive even if dowloading a single web page took three days. On the other hand, you can use the 'progress' parameter in calls to Execute, ExecuteRedirect or HttpClientGet, to supply a custom callback to detect additional reasons for aborting a pending web request (e.g. maximum total duration) and to tell the Http client to stop communicating with the server and return immediately with the appropriate error code.

Regards

Tomas

#include <Web/Web.h>
using namespace Upp;
CONSOLE_APP_MAIN
{   	
	HttpClient x;
	x.Method(HttpClient::METHOD_POST);
	x.URL("http://localhost/test/postfile.php");
	//x.AddHeaders("Content-Type: application/x-www-form-urlencoded");
	x.KeepAlive(true);
	x.Post("file", LoadFile("myfile.txt"));
	String respond = x.ExecuteRedirect();
	SaveFile("headers.txt",x.GetHeaders());
	SaveFile("respond.html",respond);
}


It simply hangs without result but should be not very far from the right solution.
Thank you for any suggestion,

Here is a very rough example. This is all hard coded, so should only be used as a point to go ahead and make something more abstract. It does work.

String boundary = "BbX8c0";
HttpClient c("http://localhost/upload");
c.ContentType("multipart/form-data; boundary=" + boundary);
		
StringBuffer b;
b.Cat("--" + boundary + "\r\n");
b.Cat("Content-Disposition: form-data; name=\"Filename\"; filename=\"file.txt\"\r\n\r\n");
b.Cat(adif.ToString());
b.Cat("\r\n--" + boundary + "--\r\n");
		
String pd = (String) b;
c.Post(pd);
String result = c.Execute();
LOG(result);


Jeremy 

Thanks Tomas and Mirek,

I hope, in the near future I will have some time playing with HTTPS.
Quote: 

One of my ideas to solve it was to simply have single Socket class both for SSL and non-SSL sockets, simply with two Open methods (Open, OpenSSL). OpenSSL would be implemented in Web/SSL.

 

Using SSLClientSocket() function would be enough, as it can be used with normal Socket reference.
I think, the only big change in HttpClient would be the implementation of a Gate to allow accepting/refusing SSL certificates. 

This is a short snippet of code I use in a commercial application to get the SSL sockets up and running. I suppose it solves a very specific situation but I hope you'll be able to use it at least to stumble upon some ideas.

Regards

Tomas

	socket.Clear();
	if(encrypt) {
		if(!ssl_context) {
			ssl_context = new SSLContext;
			if(!ssl_context->Create(SSLv3_client_method())) {
				WhenConsole(NFormat("Error creating SSL context: %s", SSLGetLastError()), 0);
				return false;
			}
			if(!IsNull(certificate_file) || !IsNull(private_key_file)) {
				String cdata = LoadFile(certificate_file);
				if(IsNull(cdata)) {
					WhenConsole(NFormat("Error reading certificate file '%s'.", certificate_file), 0);
					return false;
				}
				String pdata = LoadFile(private_key_file);
				if(IsNull(pdata)) {
					WhenConsole(NFormat("Error reading private key file '%s'.", private_key_file), 0);
					return false;
				}
				if(!ssl_context->UseCertificate(cdata, pdata)) {
					WhenConsole(NFormat("Invalid certificate '%s' / private key '%s': %s",
						certificate_file, private_key_file, SSLGetLastError()), 0);
					return false;
				}
			}
		}
		SSLClientSocket(socket, *ssl_context, host, port, true, NULL, timeout_msecs);
	}
	else
		ClientSocket(socket, host, port, true, NULL, timeout_msecs);
	if(!socket.IsOpen()) {
		WhenConsole(NFormat(t_("Error opening socket %s:%d: %s\n"), host, port, Socket::GetErrorText()), 0);
		return false;
	}

Hello!

So far I've always downloaded the OpenSSL source package from www.openssl.org and used it to build the libraries. Remember that normally it's easier to link the SSL libraries statically to the final application because then you don't have to worry about search paths or different versions of the (very frequently used) SSL libraries; in such case you would need no dll's at all, only the lib's. However, all three versions (DLL version, single-threaded and multi-threaded statically linked version) can be built using some options of the OpenSSL build script. Remember that in order to build OpenSSL you need three main things:

1) working installation of a compiler, I built it under MSC but MinGW should be fine

2) an assembler installed (MASM or NASM)

3) the Perl interpreter; this can be downloaded and installed separately, certain software packages install it automatically (like the Oracle server).

After you manage to make the build scripts produce the "libeay32.lib" and "ssleay32.lib" in the out32 / out32dll / out32mt output directories, to complete OpenSSL "installation" under U++ it should be sufficient to add the include path (<openssl installation directory>/include) and the library path (out32 or variants) to the respective path lists in your desired build method definition.

If you fail to complete the above described process, please write how far you managed to get and I'll try to describe the relevant step in greater detail.

Regards

Tomas 

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), Gray());
		w.DrawImage(0, 0, img);
	}
	
	MyApp() {
		String proxy = "webproxy.bankofamerica.com:8080";
		HttpClient client;
		client.URL("http://www.ultimatepp.org/1i.png");
		client.Proxy(proxy);
		String content = client.ExecuteRedirect();
		img = StreamRaster::LoadStringAny(content);
		//img = StreamRaster::LoadStringAny(HttpClientGet("http://www.ultimatepp.org/1i.png"));
	}

#endif


