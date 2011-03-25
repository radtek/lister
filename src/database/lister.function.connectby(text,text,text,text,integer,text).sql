CREATE OR REPLACE FUNCTION public.connectby(text, text, text, text, integer, text)
 RETURNS SETOF record
 LANGUAGE c
 STABLE STRICT
AS '$libdir/tablefunc', $function$connectby_text$function$
