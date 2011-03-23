CREATE OR REPLACE FUNCTION public.cpmove_relation(param_tblname character varying, param_source_schema character varying, param_dest_schema character varying)
 RETURNS boolean
 LANGUAGE plpgsql
AS $function$
-- param_tblname is the table name
-- param_source_schema is the source schema
-- param_dest_schema is the destination schema
DECLARE
    new_schema_oid oid;
    old_schema_oid oid;
    tblname_oid oid;
BEGIN
    new_schema_oid := (SELECT oid FROM pg_catalog.pg_namespace
                WHERE nspname = param_dest_schema);
    old_schema_oid := (SELECT oid FROM pg_catalog.pg_namespace
                WHERE nspname = param_source_schema);
                
    tblname_oid := (SELECT oid FROM pg_catalog.pg_class
                WHERE relname = param_tblname AND relnamespace = old_schema_oid);
                
    IF new_schema_oid IS NULL or old_schema_oid IS NULL or tblname_oid IS NULL THEN
        RAISE NOTICE 'schema or table is invalid';
        RETURN false;
    ELSE
        EXECUTE('ALTER TABLE ' || param_source_schema || '.' || param_tblname || ' SET SCHEMA ' || param_dest_schema);
        
    
        /**Correct postgis geometry columns **/
        IF EXISTS(SELECT table_name 
                    FROM information_schema.tables 
                    WHERE table_name = 'geometry_columns' AND table_schema = 'public') THEN
            UPDATE public.geometry_columns SET f_table_schema = param_dest_schema
                WHERE f_table_schema = param_source_schema and f_table_name = param_tblname ;
            END IF;
                                                                                    
        RETURN TRUE;
    END IF;
END
$function$
