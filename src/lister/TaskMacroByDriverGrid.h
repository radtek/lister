#ifndef _lister_lister_TaskMacroByDriverGrid_h_
#define _lister_lister_TaskMacroByDriverGrid_h_

#include <lister/Urp/UrpGrid.h>
#include <lister/Urp/UrpInput.h> // Need BoolOption

class Connection;

//==============================================================================================
class TaskMacroByDriverGrid : public UrpGrid {
public:
	typedef TaskMacroByDriverGrid CLASSNAME;
	
	EditString            fldNote;
	EditStringNotNull     fldSearchFor;
	EditStringNotNull     fldReplaceWith;
	int                   taskId; // Filter all tests by task id, for manageability
	                      // Count of fixed columns that are present for all sets
	int                   driverColumnOffset;
	                      // Count of driver columns (currently 3 x the drivers
	int                   driverColumnsPresent;
	int                   driverCount;
	
	// List of pointers to EditString objects created on the fly and destroyed in our destructor
	VectorMap<
		String, 
		EditString *>     editors;
	
                          TaskMacroByDriverGrid       ();
                         ~TaskMacroByDriverGrid       ();
	virtual void          Build                       (Connection *pconnection); // Mandatory UrpGrid implementation
	virtual void          Load                        ();                        // Mandatory UrpGrid implementation
	                      
	void                  SetTaskId(int ptaskId);
	
	                      // Added manually from appending a row.
	void                  EnterNewRow                 (); // New generic name
	                      
	                      // GridCtrl will remove the row if we do not cancel the remove.
	void                  RemoveRow                   ();
	
	                      // For When callbacks it's easier to call a no-arg function
	void                  SavePrompt                  ();
	void                  SaveNoPrompt                ();
    void                  FieldLayout                 (FieldOperator& fo);
	void                  Save                        (bool prompt);
	void                  SaveRow                     (int row, int newProcessOrder);
	bool                  MeaningfulDataChange        ();
};

#endif
