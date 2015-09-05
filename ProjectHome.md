# Project Description #
This is a free, GUI SQL script editor, processor and test runner written in C++ using the U++ free toolkit.  It is primarily designed as a tool for Business Analysts on a Database/Warehouse Procurement team.

# Primary Feature #
Mainly: fast connection, easy reconnecting, retention of all scripts, systematical storing and running of script as logged tests, and access to other non-SQL sources in a consistent interface.  Also easy management of massive number of connections and passwords.

# Reason for Not Using Existing Tools #
For Windows XP, I need a SQL tool that can start up quickly, connect quickly, run sql, save it, etc.  It also needs to support macro substitution, task tracking, and as many connection types as possible.  The minimum set is Oracle, DB2, MS SQL Server, and SyBase.   Excel, flat file, FTP, and HP Mercury Quality Center connectivity would also be nice.
Existing tools such as Toad, SQuirreL, PL/SQL Developer, SQL Plus, SQL Server Management Studio, and pgAdmin III did not fit the bill - Primarily, they were not fast!

Toad, for example, takes up many seconds to open, and then to open a connection takes up to 30 seconds over my latent network.  I have no idea what it is doing that takes so long.

# Functionality So Far #
  * List all available connections on main window.
  * Retain list of all possible connections in grid (unlike Toad, which closes the connection list after connection).
  * Support Microsoft SQL Server (Unlike Toad) (Comes with U++)
  * Support Oracle (Unlike SQL Server Management Studio) (Comes with U++, I'm enhancing)
  * Support ODBC, but need to allow user to define connection types.
  * Support PostgreSQL (Uncommon) (Comes with U++)
  * Support Sybase via ODBC (takes port # arg)
  * Support DB2 via ODBC
  * Support OS Auth (so no userid req.)
  * Sort connections easily by name
  * Show connectivity state in meaningful color
  * Display passwords in plaintext (Not available in Toad or MS, available in PL/SQL Developer)
  * Retain last SQL run in script editor (Not available in other products)
  * Remember last task and script worked with
  * List saved scripts in a searchable droplist. SQuirreL is not dynamically live search.
  * Store all metadata in free non-proprietary database. (PostgreSQL)
  * Support Oracle9+ TIMESTAMP datatype. (Haven't uploaded the driver yet.)
  * Asynchronous background connection.
  * Reconnect when objects timeout (quickly and automatically) (Toad fails, so does PL/SQL Developer)
  * Long-term support
  * Free product
  * Free for any use, including commercial (Under New BSD licence)
  * Meaningful sounds when connections or executions fail
  * Easy zoom from normal, full screen (no title bar), to multiscreen. F11.
  * Zoomable scripts with scroll wheel.  Generally support more advanced editor capabilities rather than the simplistic editors in other SQL tools.
  * Retains all user customizations around window positioning, splitter positioning.
  * Create protective XML configurations that retain many user customizations without being zapped over new releases.
  * Retain all user grid sizings for connection grid, partly in editable XML files.
  * Popup schema lookups for columns by pressing "." after the table alias. (Common in most products)
  * Generate set of SELECT statements with UNION ALL between them based on a macro that parses a list of NL separated strings on the clipboard. (Too esoteric?)
  * Generate a comma-delimited set of alias-prefixed columns, each with a commented datatype description, just after the previous SELECT keyword, by pressing CTRL-period after the table alias.
  * Copy output from the grid.
  * Interruptible fetching to the output grid (Very common, I know)
  * SVN available code.
  * Heavily documented. (Trying to make it meaningful and readable and useful :))
  * Multiple sounds at once.
  * Simple code, non-obfuscative.
  * Long variable names, well-thought-out.
  * Task management: List tasks and a note and a QC Ticket # that matches this task.  Supports add/edit/delete since using new UrpSqlGrid.
  * Enhanced U++ ODBC driver to use ExtendedFetch instead of Fetch-ing single rows, which kills a latent environment.  Resulting performance is well over 2000%.  Not exaggerating.  Of course, if you are local to a database or at work, then you won't see this improvement.
  * For Oracle, got a reconnect after timeout to work.  Popup for now asks you if you wish to reconnect and continue running script.  Pretty cool.
  * Copy column names when ctrl-c from any grid. (Not sure ArrayCtrl based grids work)
  * Created Urp Library of reusable functions.
  * Push any script grid output to a physical table on local metadatabase.
  * Access metadatabase through same connection list.
  * Cancel long runs (sometimes works)
  * Cpp/header separation style in code to improve build times.  includes in exposed headers reduced to minimal to reduce precompiler scan times.
  * Attach a script to a task.
  * Review list of scripts attached to a task and a short documentation line describing outcome/purpose/"why" for the script.  Maintains order added.
  * Copy task scripts into main editor window for execution.
  * Dynamically managed toolbar enabled buttons.  Managed very closely so that only useful buttons are enabled.
  * Attaches scripts to a task, edit the scripts and re-attach.
  * Document nature of relationship of script-task relationship
  * Order display of scripts assigned to a task.
  * Run script execution output to local table, define the name, truncate or create table based on column definition.
  * Connect locally to generated tables and apply queries
  * Copy script grid to tab-delimited text with column headers as first line
  * Copy a single block with ctrl key.
  * Varied macro support; TPLUS{n}, TMINUS{n} supported, now dropdown lists active macros, what they would substitute to and the function used.
  * Logs all generated and expanded scripts to text file (1 per session only)
  * Massive Task-centered dialog to track all details around a task
  * Add notes and description to a task
  * Assign up to 5 dependencies as a chain of contacts
  * Add dates with a drop down calendar around various task deliverables.
  * Add a list of "Elements" that will assist in understanding a task, like a list of ticket types missing, etc.
  * Document a solution suggested for a task
  * Document a url and note for documents and web pages related to a task.  Need to get url link to open.
  * Insert/Edit/Delete tasks or scripts in main screen.
  * Double-click a task to edit its detail
  * Click on a task-script to open it in editor, warn if unsaved script text in editor.
  * Run all selected scripts under a task with Ctrl-Alt-F8 command in a row, and connect to any unconnected connections as needed, and display all progress on a popup window with colored text to make more meaningful.  Scroll down as it runs to show the most current activity
  * Display rowcounts, run time, start/end time, column counts, connection name, internal script id of current run, with a running status in case it locks up.
  * Paste multi-line text directly as an IN string, comma-delimited and apostrophed! Very useful
  * Paste multi-line as a series of macro expansions with a defined separator, and filters with [[INPUT1](INPUT1.md)], [[INPUT2](INPUT2.md)] being reusable in the line output.  Really cool.
  * Paste multi-line text as single joined string.
  * Contacts Dialog with list of all contacts, emails, and notes.  Drop down in various places for assignment to tasks.
  * Macro support for T+1, T+n business date calculations with limited holiday support for US.  T-n supported, too.  TPLUSORA, TPLUSMSSQL, TPLUSMSSQLUK.
  * Environment (Prod/Dev/UAT/etc) macros can be embedded in scripts so scripts can be more easily reused across environments.
  * Nesting of macros supported.  Task-level macros are shared across all the scripts under that task.  Task macros can refer to their predecessors as well as Env macros.  Will add global macros or project macros(?) soon.
  * Added macro function for transforming US date to UK date.
  * Can move scripts up and down with drag/drop.  Will bring functionality to all grids.
  * Use context menu to copy output columns to IN clause listing, or table grid
  * Paste a list of values as an apostrophied IN clause.
  * Paste a list of columns with a user-input for column prefix and alias prefix
  * Macro embedding/expansion support. Business date and global holiday calendar support, so T+1 skips back over special days, like Christmas.  Sensitive to markets (FI holidays versus Equity holidays (not yet for these)
  * Ctrl-Z working much better to back through scripts.
  * Count the number of commas in a highlighted text segment.
  * Select scripts assigned to tasks and assign as the functional component of a test.
  * Save and edit tests.
  * Limited execution of tests.
  * Use the connection assigned to the task script when running a test.
  * Support tests that are "No test", just run to populate test harness data.
  * View each record in a vertical popup.
  * Track a list of contacts, emails and notes; attach contacts/groups to tasks
  * Assign a contact/group to a test for resolution
  * Update test Pass/Fail/Indeterminate outcome automatically
  * Record last run timestamp per test

# Planned Functionality #
Things I'm getting to, in no particular order.

## Connectivity ##
  * Support MySql and sqlite3. (Drivers available in U++)
  * Support dBase dbf files. (Drivers available in U++)
  * Support sybase with dll driver. (Not available in U++)
  * Support DB2 directly. (Not available in U++)
  * Support OLE, OLEDB. (Drivers available in U++)
  * Possibly ActiveX.
  * Support FTP. (FTP plugin with U++) (Working in sample project)
  * Support Telnet. (Working in sample project)
  * Support Telnet/FTP daisy-chaining connection and file fetch. (Working in sample project)
  * Support fixed-position flat files (Working partially in sample project)
  * Support HTTP/HTTPS. (Web/WebSSL capability in U++)
  * Support XML files. (Drivers available in U++)
  * Support reading INI files as data input.
  * Play multiple sounds across each other to inform when multiple connections succeed or fail.
  * Autosys scripts using command-line interface.
  * ErWin possibly, for metadata updating.
  * Connection server.  Run connection management as OLE/ActiveX component in background on desktop, so constant restarting of the main app doesn't constantly have to start reconnections.  App is so buggy that constant restarts are an issue.  Server would send "keep alive" scripts like "select 1 from dual" periodically, detect timeouts and reconnect automagically, detect expired passwords and regenerate new passwords based on password history (kept locally) and rules.  Use scheme like rotating letters through alphabet, so "xxr1332" would rotate to "yyr2332", which rotates 2 letters and 1 number.  Common rule for Oracle is you must change 3 characters.  Recognize all Oracle password change failure codes and avoid 3 strikes ruling.  Detect account locked and login on separate superuser account and unlock if permissible and available.
  * (ADVANCED/IMPOSSIBLE)Proxy connection server on separate server to pass connections back to local desktop so superuser accounts would only show up in connection/audit history as the other servers id.  Self-clean audit file if access.
  * Support periodic automatic review of connections and log failures, unusual delays, timeouts.  ping failed connection's server to verify not a DNS issue.
  * Autodetect VPN timeout failure when connections fail and auto-reconnect VPN in background.  Perform various support routines on reconnect to VPN (Restart Office Communicator, flush Office cache, restart Google web browser, kill IE browsers if Quality Center was active (will be corrupt))

## Script Management ##
  * Categorizable scripts in database.
  * Embed links to scripts in one another with macros, either dynamically or statically. (Not sure what form it would take, though)
  * Embed output of other scripts into scripts before sending.
  * Scheduling in autosys-style
  * Regex searchable, but with easy as well as hard searching. (U++ support, partially working on another program)
  * FTS (Full Text Search) using PostgreSQL features for complex searching of "x and y" terms. (Got it to work on another program)
  * Search automatically for plural/singular forms of words. (Got it to work on another program)
  * Choice of searching for pure text or word form of text. (Got it to work on another program)
  * Graphical markup storage of scripts (so far QTF supported, may support RTF and HTML)
  * Colorization of scripts (bold SELECT, etc)
  * SQL reformatting
  * Macro "WITH" header statements, like a declare: "[[COLSET(PREFIX) AS [[PREFIX](DECLARE.md)]X,Y,Z,H,N+1, [R2](https://code.google.com/p/lister/source/detail?r=2) D(SUBSELECT)]]".  Then it could be used " SELECT [[COLSET](COLSET.md)] union SELECT [[COLSET](COLSET.md)] FROM Y".  Could really help with complex queries that become hard to maintain.  The prefix would be picked up from a table's alias and the "dot" would be created if the alias was available, or not created if no alias was used. Hmmm.
  * Track script execution time and row count output when attached to a task. Track average execution time, average row count.
  * Macros can be applied from many fields and objects. I know, high risk of intrusion.  Woopdeedoo!

## Task Management ##
Tasks help me manage a set of scripts.  Macros are assigned to a task.
  * Create Task Driver Set, a set of macros that are applied to a Task when running its scripts.  So, one Driver Set is (Test BloombergNY) another is (BloombergAsia).  This allows reuse of scripts and tests while still tying the scripts and tests to a task.  This keeps management orderly.
  * Record output separately between runs per Task Driver.
  * Generate Reports per Task and per Task Driver Run.
  * Tasks have a default Driver initially.  User should name.

## Test Run Automation ##
A big part of BA work is running scripts over and over (and over).

  * Store scripts+connection+test information as Tests that can be rerun.
  * Support greenlight/redlight review of test runs.
  * Track all test runs.
  * Link any script output to a test pass/fail/indeterminate state.  Supported options will be rowcount, duration, column value, column datatype, error code.
  * Supported test comparison types will include equality, less than/greater than, difference, percentage difference, in set of constants, starts with, ends with, is all cap, is all lowercase.  Others as needed.
  * Connect automatically from a test (working), connect to all environments and compare output as grid.  Tests going down, environments listed across the horizontal axis.
  * Execute selected tests
  * Create tests that run on a schedule and alert on outcome
  * Detect unusual variance in output of tests, regardless of pass or fail status
  * Create a Warning status, where there are buckets of results.
  * Create mandatory pass tests, such as "must be at least 10 rows before testing".  A "stop on failure".
  * If-then test branches?

## Data Relations ##
Data is always related all over several systems.  There needs to be a way to grasp the whole of the data in simpler views.

  * View the predecessors and antecessors from the data view as various drill operations.
  * Create relationships on the fly.
  * Document data at system/table/column/value level from the output grid.
  * List tasks related to the output your viewing.
  * List tests around the output your viewing.

## Analysis ##
A big part of BA work is trying to coalesce your thoughts around a problem.  We do the same methods, but in disorganized fashion, so as to waste time rethinking.

  * Store analysis as a grid with each line item outputting a count and a total target count that is decremented at bottom of grid, calculating % resolution as you go.
  * Each analytic line item has a script with a macro for Date [[WORKDATE](WORKDATE.md)] embedded from the analysis summary level.  That way the analysis can be run for several dates.
  * At the summary level, the run default date could be [[TPLUS1](TPLUS1.md)].
  * Each line item has a status of "EXPLAINABLE-NON-ISSUE", "UNRESOLVABLE", or "RESOLVABLE", and counts per are output at bottom in a summary section.
  * Reasons could be attached to each line item, from a dropdown.  "SOURCE BUG", "AGGREGATOR BUG", "ETL BUG", "MODEL BUG", "USER MISUNDERSTANDING", "S & A DEFINITION BUG"
> > , "REQUIREMENT GATHERING BUG", "OUT OF SIGNED OFF SCOPE"
  * Status of each reason could be in place: "Source working on it/aware/not responding/delayed response/contact at source ill-defined/source disagrees as to nature/more samples required by source/unable to get samples/samples erratic/too intermittent/too performance intensive to fetch/environment-specific bug/source cannot duplicate/evidence lacking/too code-intensive to regenerate samples daily/bug extreme periodicity due to periodic nature of output/awaiting user to provide samples/source has no resources"
  * Attach user expectation as it changes to each line item: "WRITE OFF", "FIX AT LATER DATE", "MUST FIX BY", "SOLUTION ACCEPTED IF AVAILABLE BY"
  * Each line item links to a solution.  line items of course can share a solution.  Each solution must have a script that can test veracity.
  * Each solution is tracked from user acceptance of solution to sample of solution output delivered to QA/UAT/Production release/Production stable
  * Solution dependencies defined: "NEWS source must be in place or will not work", "Data must be loaded for X,Y,Z first or fix will fail", "Fix only applicable to env X"
  * Multiple interdependent solutions can be linked to each line item
  * Test history for each solution run attached.
  * Total summation show history of graph as changes grow.
  * Users can attach priorities to each line item "MUST FIX", "MUST FIX BY INITIAL DEP", "MUST FIX BY NEXT RELEASE", "LIKE TO HAVE"
  * Partial solutions could be noted, expected % or specific counts that would be fixed by a solution could be noted. Some solutions may fix a specific missing account, for instance.
  * Hypothetical solutions should be noted. % of confidence in solution.  Time factor for solution should be noted. Dependent on an unavailable new procurement, for instance.
  * Failed solutions tracked so as to avoid reusing.
  * Untestable solutions (say, until env = XXX reached) are noted.  Reasons for test unavailability noted (No time, High confidence, limited time)
  * Internal prioritization of solutions pursuit committance managed so we could secretly focus on the solutions we think are realistic even though user wishes such and such solution pursued ("My cousin Elroy can fix this, call him up")
  * Generation of attractive Excel or Word report XML that shows graph of resolved portions, history of resolution graph as a 3-d flow of 2-d bar-graphs flowing across a plane, with red-to-green status changing as solutions implemented, gray if cancelled as priority.


## Other ##
  * Reconnect silently if lost connectivity (Working, but with a popup)
  * Ping servers with dummy sql to keep connections alive (naughty!)
  * Retain current SQL in script editor if changing connection to another server that is the same line but different environment (i.e., just switching from UAT to Prod, for instance)
  * List saved scripts for current connection line in drop down.  SQuirreL lists all SQL regardless of their meaninglessness to current connection.
  * Visible source easily accessible
  * Meaningful sound effects for database activity so you can do other things while waiting for connections or SQL to return.
  * Track success of all run scripts, track timing.
  * Retain all custom settings in XML rather than binary files.
  * Track multiple connection formats, including where LDAP passwords are passed and have to be kept centrally current, or the current connection context is passed directly by Windows.
  * Maintain massive collection of connections and scan with tnsping function and verify status.
  * Track failed password statuses and note next to connection in variant color.
  * Popup warnings on restricted connections where even accidental login can cause termination, in case accidentally click on wrong connection.
  * Connect on thread so scripts and test plans can be managed without having to wait.
  * Create connection streams, a collection of connections for a project or particular test.
  * Support OCIPasswordChange requests (No products except SQLPlus supports)