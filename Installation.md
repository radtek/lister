# Introduction #

Following is a weak (lame) attempt to list what is needed to use this product.  I assume that a developer is building this for now.


# Details #
  * Only tested on Windows XP SP3
  * Postgres 9.0 for Windows (32-bit).  Probably will install 9.3 soon.
    * Added tablefunc module for crosstab. Necessary to work with macros.
    * database named postgres, user postgres, password postgres, superuser
    * 2 schemas, public and exportspace (for temp tables).
    * Database as defined in database folder constructed.
    * Will need to create some core data to get it started
  * U++ IDE and upp libs to build. v.2791. Some modifications.
  * Visual C++ 7.1
  * Parts of Microsoft SDK 7.0 or 7.1
  * Drivers
    * ODBC
    * Sybase ACE 11.5, Adaptive Server Enterprise
    * Oracle 8 or 9.  Not sure what I got to work, but it wasn't pretty.
    * Microsoft SQL Server 2005 Client, SQL Native Client
    * iseries ODBC libs for DB2/AS400 connecting. iSeries Access ODBC Driver
    * Postgres pq(?) dll
  * TortoiseSVN or some SVN client to download the code easily.