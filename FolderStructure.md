# Introduction #

Each source folder under /src is a U++ Package.


# Details #

U++ has a nice structure that keeps each "package" in a separate folder.  This allows C++ to adopt one of the cool features of Java that allows you to manage code in groups of related files.  The files are still compiled in as source code and are not treated as compiled libraries, so you don't have to decide between a logical design and a single simple build.

Packages:
  * lister - The main package.
  * Urp - My clever collection of code to extend U++ classes where I don't feel like arguing with the U++ developers about whether to dot an i or not.  This package is included into the lister build by the lister.upp file.
  * Oracle - A duplication of the U++ Oracle package but localized with my enhancements for asynchronous execution, parsing errors, and cancellable queries, as well as detecting broken connections and reconnecting.  Unfortunately properties in the Sql package were not virtual, and the U++ owner doesn't like virtual methods, so I don't ask.
  * PostgreSQL - This will probably have to be brought in, too, since it has a bug on recognizing boolean data.  Though maybe I can get U++ team to accept a change.