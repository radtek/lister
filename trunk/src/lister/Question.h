#ifndef _lister_lister_Question_h_
#define _lister_lister_Question_h_

//==============================================================================================
class Question {
public:
	int      questionId;
	String   questionText;
	String   answerText;
	String   status;
	int      taskId;
	Question(
					int     pquestionId
				,	String  pquestionText
				,	String  panswerText
				,	String	pstatus
				,	int     ptaskId
			);
};

#endif
