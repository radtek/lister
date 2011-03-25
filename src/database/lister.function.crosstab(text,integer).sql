CREATE OR REPLACE FUNCTION public.crosstab(text, integer)
 RETURNS SETOF record
 LANGUAGE c
 STABLE STRICT
AS '$libdir/tablefunc', $function$crosstab$function$
