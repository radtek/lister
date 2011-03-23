CREATE OR REPLACE FUNCTION public.getnewlogin()
 RETURNS integer
 LANGUAGE plpgsql
AS $function$
DECLARE
   floginid INTEGER;
BEGIN
   SELECT loginid INTO floginid FROM logins WHERE loginstr = new.loginstr and loginpwd = new.loginpwd;
   RETURN 0;
END;
$function$
