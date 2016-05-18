/*
 *  Mr Pass.  Brain the size of a planet!
 *
 *  Proundly Created by Richard Buckland
 *  Share Freely Creative Commons SA-BY-NC 3.0.
 *
 */
 //The writing boy
 //Modified by John Cawood on 18/5/16

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

   path aiPathARC = {0};

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


   path aiPathCampus = {0};

   //next part is simialer to arc pathing above
   //but since 2 campuses can't be made adjacent it 
   //works slightly differently
   //needs to be checked

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
   

   action nextAction;
   int actionChosen = FALSE;

  if(turn != NO_ONE ) {
   
   //Build Campus
    nextAction.actionCode = BUILD_CAMPUS;
    strncpy(nextAction.destination, aiPathCampus, PATH_LIMIT);
    
    if (isLegalAction(g, nextAction)) {
      actionChosen = TRUE;
    }
   
   //Build Arc
   nextAction.actionCode = OBTAIN_ARC;
   strncpy(nextAction.destination, aiPathARC, PATH_LIMIT);

   if (!actionChosen && isLegalAction(g, nextAction) == TRUE) {
      actionChosen = TRUE;
   }
   
   //Spinoff
   nextAction.actionCode = START_SPINOFF;

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
