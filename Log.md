# Details #

  * February, 2011
    * Fixed DB2/AS400 connectivity using iSeries ODBC driver.
    * Got more data types to work with fetch arrays, SQLINTEGER.
  * March, 2011
    * Added Sybase ASE 12 connection support.
    * Added automatic OS Authorization for MS SQL Server ODBC connection.
    * Added Macro support at task level so all scripts attached to a task see those macros.
    * Added editing support for task macros, insert/delete/change.
    * Added Ctrl+S hotkey support for scripts, and F5 to run scripts.
    * Added port # attribute support for Sybase and MS SQL Server.
    * Added toolbar button to connect & run script.
    * Fixed more ODBC data types, including TINY\_INT and BIGINT.
    * Refactored relationship between ODBC binding variables and fetching their content.
    * Added Environment macros that follow the connection environment for customizing scripts to the environment.
    * Added a dropdown list of active, available macros for the user to select and add to script.
    * Recalculate macros at script level each time a script is selected.
    * Restarted work on TestGrid. Pass/Fail is flagged, different test types, timing is tracked.
    * All tests listed in the Test Window Grid are linked to the active Task for simplifying management.
    * Scripts listed in the TestGrid are now members of the Scripts, selected from a drop down.
    * UrpGrid and UrpSqlGrid now assist with persisting row order to the database for grid data.
    * Created RecordView popup for OutputGrid.
    * Added HTML Excel fragment copy support to UrpGrid so that grids can be copied and directly pasted to outlook.
    * Added toolbar to test window to start adding complex user controls to tests.
    * Added colorization to test rows to show current outcome of a test run: Red/Yellow/Green or white.
    * Added database folder and uploaded all DDL SQL for tables, views, view rules, and functions. no timestamps.
    * Added doc folder with some legacy documentation.
    * Create database export function to periodically update the SQL.  SQL is per object so as to easily detect change.