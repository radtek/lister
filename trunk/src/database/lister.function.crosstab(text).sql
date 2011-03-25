CREATE OR REPLACE FUNCTION public.crosstab(text)
 RETURNS SETOF record
 LANGUAGE c
 STABLE STRICT
AS '$libdir/tablefunc', $function$crosstab$function$
