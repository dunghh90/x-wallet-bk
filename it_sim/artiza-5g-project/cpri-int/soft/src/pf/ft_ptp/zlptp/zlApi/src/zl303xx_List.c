

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Functions to implement a doubly-linked list.
*
*******************************************************************************/

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_List.h"

/*****************   DEFINES     **********************************************/

/*****************   STATIC FUNCTION DECLARATIONS   ***************************/

/*****************   STATIC GLOBAL VARIABLES   ********************************/

/*****************   EXPORTED GLOBAL VARIABLES   ******************************/

/*****************   EXPORTED FUNCTION DEFINITIONS   **************************/

/* zl303xx_ListInvalidate */
/** 

   Invalidates a list entry by setting its prev and next pointers to NULL.
   List entry validity can be checked using zl303xx_ListValid().

  Parameters:
   [in]   entry   List entry to invalidate.

*******************************************************************************/
void zl303xx_ListInvalidate(zl303xx_ListS *entry)
{
   entry->prev = NULL;
   entry->next = NULL;
}

/* zl303xx_ListInit */
/** 

   Initializes a linked list head to the empty set.

  Parameters:
   [in]   head   Linked list head to initialize.

*******************************************************************************/
void zl303xx_ListInit(zl303xx_ListS *head)
{
   head->prev = head;
   head->next = head;
}

/* zl303xx_ListAdd */
/** 

   Adds an entry to the beginning of a linked list.

  Parameters:
   [in]   head   Head of a linked list.
   [in]   add    The entry to add.

*******************************************************************************/
void zl303xx_ListAdd(zl303xx_ListS *head, zl303xx_ListS *add)
{
   /* All parts of the HEAD should be initialized. */
   if (zl303xx_ListValid(head) == ZL303XX_FALSE)
   {
      zl303xx_ListInit(head);
   }

   add->prev = head;
   add->next = head->next;
   head->next->prev = add;
   head->next = add;
}

/* zl303xx_ListAddTail */
/** 

   Adds an entry to the end of a linked list.

  Parameters:
   [in]   head   Head of a linked list.
   [in]   add    The entry to add.

*******************************************************************************/
void zl303xx_ListAddTail(zl303xx_ListS *head, zl303xx_ListS *add)
{
   /* All parts of the HEAD should be initialized. */
   if (zl303xx_ListValid(head) == ZL303XX_FALSE)
   {
      zl303xx_ListInit(head);
   }
   
   add->prev = head->prev;
   add->next = head;
   head->prev->next = add;
   head->prev = add;
}

/* zl303xx_ListRemove */
/** 

   Removes an entry from a linked list. Does not check if the entry actually
   exists within a specific linked list. Does nothing if the provided list
   entry is invalid.

  Parameters:
   [in,out]  rem   The entry to remove. Its prev and next pointers will be
                        set to NULL.

*******************************************************************************/
void zl303xx_ListRemove(zl303xx_ListS *rem)
{
   if (zl303xx_ListValid(rem))
   {
      rem->prev->next = rem->next;
      rem->next->prev = rem->prev;
      zl303xx_ListInvalidate(rem);
   }
}

/* zl303xx_ListRemoveFrom */
/** 

   Removes an entry from a linked list if it exists within a specified list.
   Does nothing if the provided list entry is invalid.

  Parameters:
   [in]      head   Head of a linked list.
   [in,out]  rem    The entry to remove. Its prev and next pointers will be
                        set to NULL.

*******************************************************************************/
void zl303xx_ListRemoveFrom(zl303xx_ListS *head, zl303xx_ListS *rem)
{
   if (zl303xx_ListExists(head, rem))   /* Checks if rem is valid */
   {
      rem->prev->next = rem->next;
      rem->next->prev = rem->prev;
      zl303xx_ListInvalidate(rem);
   }
}

/* zl303xx_ListValid */
/** 

   Determines if a linked list entry is valid by check if its next pointer is
   not NULL. A valid list entry is either the head of a list (can be empty) or
   an element within a list.

  Parameters:
   [in]   entry   List entry to validate.

  Return Value:
     ZL303XX_FALSE  The list entry is not valid.
     ZL303XX_TRUE   The list entry is valid.

*******************************************************************************/
zl303xx_BooleanE zl303xx_ListValid(zl303xx_ListS *entry)
{
   return ((entry->next != NULL) && (entry->prev != NULL));
}

/* zl303xx_ListEmpty */
/** 

   Checks if a linked list is empty.

  Parameters:
   [in]   head   Head of the linked list.

  Return Value:
     ZL303XX_FALSE  The list has at least one element.
     ZL303XX_TRUE   The list is empty.

*******************************************************************************/
zl303xx_BooleanE zl303xx_ListEmpty(zl303xx_ListS *head)
{
   return (head->next == head);
}

/* zl303xx_ListExists */
/** 

   Determines if a specified entry exists within a linked list.

  Parameters:
   [in]   head    Head of the linked list.
   [in]   entry   The entry to search for.

  Return Value:
     ZL303XX_FALSE  entry is invalid or it does not exist in the list.
     ZL303XX_TRUE   entry exists in the list.

*******************************************************************************/
zl303xx_BooleanE zl303xx_ListExists(zl303xx_ListS *head, zl303xx_ListS *entry)
{
   zl303xx_ListS *curr;

   if (!zl303xx_ListValid(entry))
      return ZL303XX_FALSE;

   ZL303XX_LIST_FOR_EACH(head, curr)
   {
      if (curr == entry)
         return ZL303XX_TRUE;
   }

   return ZL303XX_FALSE;
}

/*****************   STATIC FUNCTION DEFINITIONS   ****************************/

/*****************   END   ****************************************************/
