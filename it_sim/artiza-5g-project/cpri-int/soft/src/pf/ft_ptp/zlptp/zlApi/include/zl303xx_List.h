

/*******************************************************************************
*
*  $Id: 5f5cd1afe103978190dd9cb30e1e731412eee2fa
*
*  Copyright 2006-2018 Microsemi Semiconductor Limited.
*  All rights reserved.
*
*  Module Description:
*     Doubly-linked list API.
*
*******************************************************************************/

#ifndef ZL303XX_LIST_H_
#define ZL303XX_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************   INCLUDE FILES   ******************************************/
#include "zl303xx_Global.h"
#include "zl303xx_Macros.h"

/*****************   DEFINES   ************************************************/

/*****************   DATA TYPES   *********************************************/

/*****************   DATA STRUCTURES   ****************************************/

/* This structure has two purposes:
 *    - It serves as the "dummy" or "sentinel" node used to represent the head
 *      of a linked list.
 *    - It can be embedded within another structure to make that structure
 *      usable as a linked list.   */
typedef struct zl303xx_ListS
{
   struct zl303xx_ListS *prev, *next;
} zl303xx_ListS;

/*****************   EXPORTED GLOBAL VARIABLE DECLARATIONS   ******************/

/*****************   EXTERNAL FUNCTION DECLARATIONS   *************************/
#define ZL303XX_LIST_HEAD_INIT(head)  {&(head), &(head)}

void zl303xx_ListInvalidate(zl303xx_ListS *entry);
void zl303xx_ListInit(zl303xx_ListS *head);
void zl303xx_ListAdd(zl303xx_ListS *head, zl303xx_ListS *add);
void zl303xx_ListAddTail(zl303xx_ListS *head, zl303xx_ListS *add);
void zl303xx_ListRemove(zl303xx_ListS *rem);
void zl303xx_ListRemoveFrom(zl303xx_ListS *head, zl303xx_ListS *rem);

zl303xx_BooleanE zl303xx_ListValid(zl303xx_ListS *entry);
zl303xx_BooleanE zl303xx_ListEmpty(zl303xx_ListS *head);
zl303xx_BooleanE zl303xx_ListExists(zl303xx_ListS *head, zl303xx_ListS *entry);

/* Macros to iterate over all entries in a linked list. Since these expand into
 * for() loops, break and continue statements can be used. */ 

/* Iterate over all entries in a linked list.

  Parameters:
   [in]   head   Head of the linked list (zl303xx_ListS *).
     [in]   curr   Current linked list entry (zl303xx_ListS *).  */
#define ZL303XX_LIST_FOR_EACH(head, curr) \
           for (curr = (head)->next; curr != head; curr = (curr)->next)

/* Iterate over all entries in a linked list. This version is protected against
  the current list entry being removed inside the loop.

  Parameters:
   [in]   head   Head of the linked list (zl303xx_ListS *).
     [in]   curr   Current linked list entry (zl303xx_ListS *).
     [in]   temp   Temporary pointer to protect current entry (zl303xx_ListS *).  */
#define ZL303XX_LIST_FOR_EACH_SAFE(head, curr, temp)        \
           for (curr = (head)->next, temp = (curr)->next; \
                curr != head;                             \
                curr = temp, temp = (temp)->next)

/* Iterate over all entries in a linked list. This version is useful when the
  zl303xx_ListS is contained within another structure.

  Parameters:
   [in]   head     Head of the linked list (zl303xx_ListS *).
     [in]   curr     Current linked list entry (type *).
     [in]   type     Type of curr.
     [in]   member   Name of the zl303xx_ListS member in type.   */
#define ZL303XX_LIST_FOR_EACH_TYPE(head, curr, type, member)                     \
           for (curr = ZL303XX_CONTAINER_GET((head)->next, type, member);        \
                &(curr)->member != head;                                       \
                curr = ZL303XX_CONTAINER_GET((curr)->member.next, type, member))

/* Iterate over all entries in a linked list. This version is useful
  when the zl303xx_ListS is contained within another structure, and it is safe
  to use when the current list entry is removed inside the loop.

  Parameters:
   [in]   head     Head of the linked list.
     [in]   curr     Current linked list entry (type *).
     [in]   temp     Temporary pointer to protect current entry (type *).
     [in]   type     Type of curr & temp.
     [in]   member   Name of the zl303xx_ListS member in curr.   */
#define ZL303XX_LIST_FOR_EACH_TYPE_SAFE(head, curr, temp, type, member)             \
           for (curr = ZL303XX_CONTAINER_GET((head)->next, type, member),           \
                   temp = ZL303XX_CONTAINER_GET((curr)->member.next, type, member); \
                &(curr)->member != head;                                          \
                curr = temp,                                                      \
                   temp = ZL303XX_CONTAINER_GET((temp)->member.next, type, member))

#ifdef __cplusplus
}
#endif

#endif /* MULTIPLE INCLUDE BARRIER */
