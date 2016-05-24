/*
 *  Mr Pass.  Brain the size of a planet!
 *
 *  Proundly Created by Richard Buckland
 *  Share Freely Creative Commons SA-BY-NC 3.0.
 *
 */
 //The writing boy
 //Modified by John Cawood on 20/5/16

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Game.h"
#include "mechanicalTurk.h"

/*
typedef struct _action {
   int actionCode;  // see #defines above
   path destination; // if the action operates on a vertex or ARC this
                     // specifies *which* vertex or path.  unused 
                     // otherwise
   int disciplineFrom;  // used for the retrain students action
   int disciplineTo;    // used for the retrain students action
} action;
*/
   
action decideAction (Game g) {

   int turn = getWhoseTurn(g);

    //determining resources, getStudents (Game g, int player, int discipline);
   //int THDs = getStudents(g, turn, STUDENT_THD);
   int BPSs = getStudents(g, turn, STUDENT_BPS);
   int BQNs = getStudents(g, turn, STUDENT_BQN);
   int MJs = getStudents(g, turn, STUDENT_MJ);
   int MTVs = getStudents(g, turn, STUDENT_MTV); 
   int MMONEYs = getStudents(g, turn, STUDENT_MMONEY);
   
   action nextAction;
   int actionChosen = FALSE;

   path aiPathARC = {0};
   path aiPathCampus = {0};

  if(turn != NO_ONE ) {

   if (!actionChosen && BQNs >= 1 && BPSs >= 1 && MJs >= 1 && MTVs >= 1) {

   //next part is simialer to arc pathing above
   //but since 2 campuses can't be made adjacent it 
   //works slightly differently
   //needs to be checked

   //for campus 1 BPS, 1 BQN, 1 MJ, 1 MTV
   if (turn == UNI_B) {
      strcpy(aiPathCampus, "RRLRLLL");
      char *next = "RL";
   
      int i = 0;
      while (i <= 30 && getCampus(g, aiPathCampus) != NO_ONE){

          strcat(aiPathCampus, next);
          i++;
      }
   } else if (turn == UNI_C) {
      strcpy(aiPathCampus, "LRLRLB");
      char *next = "LR";
   
      int i = 0;
      while (i <= 30 && getCampus(g, aiPathCampus) != NO_ONE){
      
         strcat(aiPathCampus, next);
         i++;
      }
   
   } else {
      strcpy(aiPathCampus, "R");
      char *next = "LR";
      
      int i = 0;
      while (i <= 30 && getCampus(g, aiPathCampus) != NO_ONE) {

         strcat(aiPathCampus, next);
         i++;
      }
    }
  
   
   //Build Campus
    nextAction.actionCode = BUILD_CAMPUS;
    strncpy(nextAction.destination, aiPathCampus, PATH_LIMIT);
    
    if (isLegalAction(g, nextAction)) {
      actionChosen = TRUE;
    }
  }

  if (!actionChosen && BQNs >= 1 && BPSs >= 1) {
    //for arc 1 BQN and 1 BPS
   
   
   if (turn == UNI_B) {
      strcpy(aiPathARC, "RRLRLL");
      char prev = 'L';
      char *next = 0;
    
      if (getARC(g, aiPathARC) != NO_ONE) {
         strcat(aiPathARC, "L");
      } else {
         strcat(aiPathARC, "L");

         int i = 0;
         while (i <= 30 && getARC(g, aiPathARC) != NO_ONE) {
           if (prev == 'R') {
               next = "L";
               prev = 'L';
           } else {
               next = "R";
               prev = 'R';
           }
           strcat(aiPathARC, next);
           i++;
         }
      }  
   } else if (turn == UNI_C) {
      strcpy(aiPathARC, "LRLRLB");
      char prev = 'B';
      char *next = 0;

      int i = 0;
      while (i <= 30 && getARC(g, aiPathARC) != NO_ONE) {
        if (prev == 'R') {
            next = "L";
            prev = 'L';
        } else if (prev == 'L') {
            next = "R";
            prev = 'R';
        } else {
            next = "L";
            prev = 'L';
        }
        strcat(aiPathARC, next);
        i++;
      } 
   } else {
    //for the first arc
      strcpy(aiPathARC, "R");
      char prev = 'R';
      char *next = 0;

      int i = 0;
      while (i <= 30 && getARC(g, aiPathARC) != NO_ONE) {
        if (prev == 'R'){
            next = "L";
            prev = 'L';
        } else if (prev == 'L') {
            next = "R";
            prev = 'R';
        } else {
            next = "L";
            prev = 'L';
        }
        strcat(aiPathARC, next);
        i++;
       }
    }
  
   
   //Build Arc
   nextAction.actionCode = OBTAIN_ARC;
   strncpy(nextAction.destination, aiPathARC, PATH_LIMIT);
   

   if (!actionChosen && isLegalAction(g, nextAction) == TRUE) {
      actionChosen = TRUE;
   } 
  }

   //for resources (prioritising arc grants)
   if (!actionChosen) {
      if (getARCs(g, turn) <= 9) {
      //at start generates MTVs and MJs
      nextAction.actionCode = RETRAIN_STUDENTS;
      if (MTVs >= 3) {
        nextAction.disciplineFrom = STUDENT_MTV;
        if (BQNs == 1) {
          nextAction.disciplineTo = STUDENT_BPS; 
        } else {
          nextAction.disciplineTo = STUDENT_BQN;
        }
      } else  if (MJs >= 3) {
        nextAction.disciplineFrom = STUDENT_MJ;
        if (BQNs == 1) {
          nextAction.disciplineTo = STUDENT_BPS; 
        } else {
          nextAction.disciplineTo = STUDENT_BQN;
        }
      } else if (MMONEYs >= 3) {
        nextAction.disciplineFrom = STUDENT_MMONEY;
        if (BQNs == 1) {
          nextAction.disciplineTo = STUDENT_BPS; 
        } else {
          nextAction.disciplineTo = STUDENT_BQN;
        }
      }
      }
    
      if (isLegalAction(g, nextAction) == TRUE) {
         actionChosen = TRUE;
      }
    }
    
  

  //builds go8s if possible
  if (!actionChosen && MJs >= 2 && MMONEYs >= 3 && getCampuses(g, turn) >= 2) {
     nextAction.actionCode = BUILD_GO8;
     path aiPathGO8 = {0};
     
     if (turn == UNI_A) {
        strcpy(aiPathGO8, "RB");
        if (getCampus(g, aiPathGO8) == CAMPUS_A){
          strncpy(nextAction.destination, aiPathGO8, PATH_LIMIT);
          actionChosen = TRUE;
        }

     } else if (turn == UNI_B) {
      strcpy(aiPathGO8, "RRLRL");
        if (getCampus(g, aiPathGO8) == CAMPUS_B){
          strncpy(nextAction.destination, aiPathGO8, PATH_LIMIT);
          actionChosen = TRUE;
        }


     } else {
       strcpy(aiPathGO8, "LRLRL");
        if (getCampus(g, aiPathGO8) == CAMPUS_B){
          strncpy(nextAction.destination, aiPathGO8, PATH_LIMIT);
          actionChosen = TRUE;
        }
     }
  }
   
   //Spinoff
   if (!actionChosen) {
    nextAction.actionCode = START_SPINOFF;
   }

   if (!actionChosen && isLegalAction(g, nextAction) == TRUE) {
      actionChosen = TRUE;
   }
   //before pass when we can we should add something that can
   //convert students so we can do more of the above actions
   //(maybe a while loop encompasing all actions)

   //Pass
   if (!actionChosen) {
      nextAction.actionCode = PASS;
   }
  
  }
     return nextAction;
}
