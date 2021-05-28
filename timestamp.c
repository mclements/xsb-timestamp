#include "xsb_config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef WIN_NT
#define XSB_DLL
#endif

#include "auxlry.h"
/* context.h is necessary for the type of a thread context. */
#include "context.h"
#include "cinterf.h"


/* call as: localdatetime(-Timestamp)
   Return the local timestamp as the number of seconds from
   1970-01-01 00:00:00 GMT.
   This is equivalent to:
      :- import epoch_seconds/1 from machine.
   As noted for epoch_seconds, this may cause overflow for 32 bit architectures.
   The function name is taken from Postgresql.
*/
DllExport int call_conv localdatetime(CTXTdecl)
{
  extern_ctop_int(1,time(0));
  return TRUE;
}

/* call as: to_timestamp(+String,+Format,-Timestamp)
   Will take String and Format and return the timestamp. 
   The Format is as per strptime() in <time.h>.
   Both String and Format must be atoms.
*/
DllExport int call_conv to_timestamp(CTXTdecl)
{
  char	*string_in; 
  char	*format_in;
  int timestamp;
  struct tm tmInfo = {0};
  
  string_in = (char *) extern_ptoc_string(1);
  format_in = (char *) extern_ptoc_string(2);
  strptime(string_in, format_in, &tmInfo);
  timestamp = mktime(&tmInfo);
  extern_ctop_int(3,timestamp);

  return TRUE;
}

/* call as: from_date(+String,-Timestamp)
   Will take String and, assuming Format='%Y-%m-%d', return the timestamp. 
   String must be an atom.
*/
DllExport int call_conv from_date(CTXTdecl)
{
  char	*string_in; 
  char	*format = "%Y-%m-%d";
  int timestamp;
  struct tm tmInfo = {0};
  
  string_in = (char *) extern_ptoc_string(1);
  strptime(string_in, format, &tmInfo);
  timestamp = mktime(&tmInfo);
  extern_ctop_int(2,timestamp);

  return TRUE;
}

/* call as: timestamps_age(+FinishTimestamp,+BirthTimestamp,-AgeInt)
   Will take the FinishTimestamp and BirthTimestamp and return the AgeInt as an integer. 
*/
DllExport int call_conv timestamps_age(CTXTdecl)
{
  time_t event = extern_ptoc_int(1), birth = extern_ptoc_int(2);
  struct tm tmEvent = *localtime(&event), tmBirth = *localtime(&birth);
  int a = tmEvent.tm_year - tmBirth.tm_year;
  if ((tmEvent.tm_mon < tmBirth.tm_mon) ||
      ((tmEvent.tm_mon == tmBirth.tm_mon) && (tmEvent.tm_mday < tmBirth.tm_mday)))
    a -= 1;
  extern_ctop_int(3,a);
  return TRUE;
}

/* call as: from_timestamp(+Timestamp,+Format,-String)
   Will take a Timestamp and Format and return a String. 
   The Format is as per strptime() in <time.h>.
   Format must be an atom.
*/
DllExport int call_conv from_timestamp(CTXTdecl)
{
  time_t timestamp_in = extern_ptoc_int(1);
  char *format_in = (char *) extern_ptoc_string(2);
  char * buf_out = (char *)malloc(101*sizeof(char));; // is this memory safe?
  strftime(buf_out, 100, format_in, localtime(&timestamp_in));
  /* Now that we have constructed a new symbol, we must ensure that it
     appears in the symbol table.  This can be done using function
     string_find() that searches the symbol table for the symbol, and
     if the symbol does not appear there, it inserts it.  If we are 
     sure that the symbol already appeared in the symbol table there
     is no need to use string_find(). 
  */
  //	extern_ctop_string(3, (char *) string_find(buf_out,3));
  extern_ctop_string(CTXTc 3, buf_out);

	return TRUE;
  }

/* call as: to_date(+Timestamp,-String)
   Will take a Timestamp and, assuming Format='%Y-%m-%d', return a String. 
*/
DllExport int call_conv to_date(CTXTdecl)
{
  time_t timestamp_in = extern_ptoc_int(1);
  char *format = "%Y-%m-%d";
  char * buf_out = (char *)malloc(101*sizeof(char));; // is this memory safe?
  strftime(buf_out, 100, format, localtime(&timestamp_in));
  /* Now that we have constructed a new symbol, we must ensure that it
     appears in the symbol table.  This can be done using function
     string_find() that searches the symbol table for the symbol, and
     if the symbol does not appear there, it inserts it.  If we are 
     sure that the symbol already appeared in the symbol table there
     is no need to use string_find(). 
  */
  //	extern_ctop_string(2, (char *) string_find(buf_out,3));
  extern_ctop_string(CTXTc 2, buf_out);

  return TRUE;
}

int localdiff() {
  time_t rawtime = time(NULL);
  struct tm *ptm = gmtime(&rawtime);
  // Request that mktime() looks up dst in timezone database
  ptm->tm_isdst = -1;                
  time_t gmt = mktime(ptm);
  return difftime(rawtime, gmt)-3600*ptm->tm_isdst;
}

/* call as: to_days(+Timestamp,-DaysInt)
   Return the local timestamp as the number of days from 1970-01-01.
*/
DllExport int call_conv to_days(CTXTdecl)
{
  int timestamp_in = extern_ptoc_int(1);
  extern_ctop_int(2,(timestamp_in+localdiff())/86400);
  return TRUE;
}
