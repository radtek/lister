# Introduction #

The drivers behind how functionality is prioritized, how it is implemented, tuned, tested - this is very good to know with any project, and is often hidden. At work the idea of "Why" we do a project is avoided since the Why could change, or the need could go away, or be resolved by some other project that completes sooner, or a vendor product becomes better or cheaper.  So if the Why was written down, it would be easy to shoot it down, and poof!  There goes the funding for your cool project.

# Details #

So here's a few of my drivers:
  * Fast.  I like to connect fast, work fast.
  * Transparency. Open source has greatly improved this, as well as the lawsuits that opened up Microsoft a bit.  I use TOAD and wonder why it takes so damn long to start up.  That was a major driver for creating lister.  Sure enough, after creating the initial connections, I realized there is no reason for connections to take so long.  I don't need it to load up the entire schema!  Wait!  I may just be running a straight query!
  * Inline Documentation.  By which I mean text actually in the code.  The best point for key documentation is at the point of implementation, not in some book or page elsewhere.  So many companies spend fortunes on documenting, having idiots write documentation about code that they have no idea what it does.  Developers: Document.  Learn how to spell.  Learn how to communicate in written form.
  * Unit test.  Great idea.
  * Honesty.  When I document, I'll tell you why something is a kludge.  I'll say why some function from U++ sucks.  I know it's free, I am grateful, but we need to call a spade a spade, and not get our feathers ruffled when someone calls us out.
  * Confession.  A part of honesty, but often left out.  I confess that much of my code sucks!  I try to fix it. I try to improve it, but sometimes expediency is more important than perfect code.
  * Use libraries.  I don't insist everything must fit a paradigm.  I'm glad U++ is so strict, but that's fine for the layer of code they represent.  At the app level, I need to take functionality as I can find it, whether or not it follows the patterns of U++, nor do I have time to convert it.
  * Steal code.  I have no shame.  If I see code, its mine. Shoot me.
  * Logs.  Log output.
  * Exceptions.  I don't use try/catch, but I do throw exceptions when something blows chunks, even in release code.
  * Online Source Control.  svn is great and provides as much visibility into my code as I could possibly need.