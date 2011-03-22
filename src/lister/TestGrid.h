#ifndef _TestGrid_h_
#define _TestGrid_h_

#include "ConnectedCtrl.h"
#include <lister/Urp/UrpGrid.h>

class Connection;

// Taken from testtyps table

#define TESTTYP_UNKNOWN                              -1
#define TESTTYP_RETURNED_ROW_COUNT                    1
#define TESTTYP_SELECTED_ROW_COUNT                    2
#define TESTTYP_ERROR_CODE                            3
#define TESTTYP_GENERIC_ERROR_CODE                    4
#define TESTTYP_FIRST_COLUMN_VALUE                    5
#define TESTTYP_FIRST_COLUMNS_DATATYPE                6
#define TESTTYP_FIRST_COLUMNS_GENERIC_DATATYPE        7
#define TESTTYP_FIRST_COLUMNS_DATALENGTH              8
#define TESTTYP_EXECUTION_TIME                        9
#define TESTTYP_PARSE_ERROR_CODE                     10
#define TESTTYP_PARSE_GENERIC_ERROR_CODE             11
#define TESTTYP_SCRIPT_TEXT                          12
#define TESTTYP_SCRIPT_TEXT_CHARACTER_COUNT          13
#define TESTTYP_SCRIPT_TEXT_WORD_COUNT               14
#define TESTTYP_SCRIPT_TEXT_LINE_COUNT               15
#define TESTTYP_SCRIPT_TEXT_LINE_WORD_COUNT          16
#define TESTTYP_SCRIPT_TEXT_LINE_CHARACTER_COUNT     17
#define TESTTYP_NOTEST                               18

// Comparison types when testing (see tests table)
#define COMPTYP_UNKNOWN                   -1
#define COMPTYP_GREATER_THAN_X             1
#define COMPTYP_EQUAL_TO                   2
#define COMPTYP_LESS_THAN_X                3
#define COMPTYP_GREATER_THAN_OR_EQUAL_TO_X 4
#define COMPTYP_LESS_THAN_OR_EQUAL_TO_X    5
#define COMPTYP_STARTS_WITH_X              6
#define COMPTYP_START_WITH_X_AFTER_TRIM    7
#define COMPTYP_ENDS_WITH_X                8
#define COMPTYP_ENDS_WITH_X_AFTER_TRIM     9
#define COMPTYP_IN_LIST_X                 10
#define COMPTYP_IN_SERIES_X               11
#define COMPTYP_CONTAINED_WITHIN_X        12
#define COMPTYP_CONTAINS_X                13
#define COMPTYP_BETWEEN_X_AND_Y           14
#define COMPTYP_WITHIN_X_PCT_OF_Y         15
#define COMPTYP_WITHIN_X_OF_Y             16

//==============================================================================================
class TestGrid : public UrpGrid, ConnectedCtrl {
public:
	typedef TestGrid CLASSNAME;
	
	EditInt           fldTestId;
	EditStringNotNull fldTestName;  // 0) A meaningful name; too many tests to track without a name
	EditString        fldTestNote;
	EditString        fldCompareUsingX;
	EditString        fldCompareUsingY;
	EditTime          fldLastRunWhen;
	DropGrid          connList
	,                 scriptList
	,                 invertCompList
	,                 desiredOutcomeList
	,                 actualOutcomeList
	,                 envList
	,                 testTypList
	,                 compTypList
	,                 assignToWhoList
	;

	int               taskId; // Filter all tests by task id, for manageability
	
                      TestGrid();
	                  // Added manually from appending a row.
	void              Build();
	virtual void      Load(Connection *pconnection);
	void              NewTest();
	                  // GridCtrl will remove the row if we do not cancel the remove.
	void              RemoveTest();
	                  // Added from the Script Editor, which only provides these three pieces of info.
	                  // Create a bare-bones frame for a new test based on a script with a valid scriptid.
	void              AddTest(String script, int scriptId, int connId);
	                  // For When callbacks its easier to call a no-arg function
	void              SaveTestPrompt();
	void              SaveTestNoPrompt();
    void              FieldLayout(FieldOperator& fo);
	void              SaveTest(bool prompt);
	int               GetTestId           (int row); void SetTestId(int row, int ptestId);
	String            GetTestName         (int row);
	String            GetTestNote         (int row);
	int               GetTestRelId        (int row);
	int               GetTestConnId       (int row);
	int               GetTestTypId        (int row);
	bool              GetInvertComparison (int row);
	int               GetCompTypId        (int row);
	String            GetCompareUsingX    (int row);
	String            GetCompareUsingY    (int row);
	String            GetDesiredOutcome   (int row);
	String            GetActualOutcome    (int row);  
	void              SetActualOutcome    (int row, String pactualOutcome);
	String            GetOutputValue      (int row);
	void              SetOutputValue      (int row, String poutputValue);
	int               GetTaskId           (int row);
	int               GetProcessOrder     (int row);
	void              SetLastRunWhen      (int row, Time plastRunWhen);
	bool              MeaningfulDataChange();
	bool              WasTestRequested();
};

#endif
