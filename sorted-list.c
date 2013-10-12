#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf) {
	SortedListPtr list = malloc(sizeof(struct SortedList)); /*malloce space for the list*/
	if(list != NULL) {										/*if the list was created successfully*/
		list->data = NULL;									/*fill in it's default fields*/
		list->next = NULL; 
		list->comparator = cf;
		list->deleted = 1;
	}
	return list;											/*return the list*/
}

void SLDestroy(SortedListPtr list) {
	SortedListPtr temp, temp2;	/*Pointers used to parse through the list*/
	temp = temp2 = list;		/*Point the pointers to the start*/
	while(temp != NULL){		/*While you haven't reached the end of the list*/
		temp2 = temp->next;		/*Move the other pointer to the next node*/
		if(temp2 == NULL){		/*if next node doesn't exist that means you're at the end*/
			free(temp);			/*Free that node*/
			break;
		}
		else{					/*else the list still has more stuff*/
			free(temp);			/*Free the node itself*/
			temp = temp2;		/*Move onto the next node in the list*/
		}
	}	
}

int SLRemove(SortedListPtr list, void *newObj){
	SortedListPtr temp;	/*Pointer used to parse through the list*/
	int compv, successval = 0;	/*compv is the value of comparison, successval is value of removal*/
	temp = list;		/*Point the pointers to the beginning of the list*/
	if(temp == NULL)
	{
		return successval;		/*If the list is empty just return*/
	}
	else
	{					/*if not check the list*/
		while(temp != NULL)
		{
			compv = list->comparator(temp->data, newObj);
			if(compv == 0)		/*found an object to delete*/
			{
				do {	/*delete more of the same object*/
					successval = 1;
					temp->deleted = 0;	/*set delete flag*/
					if((temp = temp->next) == NULL)
						break;	/*break out of while loop if there are no more objects*/
				} while(list->comparator(temp->data, newObj) == 0);	/*checks if there are still more of the same object*/
				break;	/*no more of the same object to delete*/
			}
			temp = temp->next;
		}
		return successval;
	}
}

int SLInsert(SortedListPtr list, void *newObj) {		/*takes in a list and void pointer*/
	SortedListPtr prev = NULL;							/*point a previous pointer to null*/
	SortedListPtr temp = list;							/*point a temp pointer to the beginning of the list*/
	int compval;										/*variable that will hold the compare function's return*/
	if(list->data == NULL){								/*If the list is empty*/
		list->data = newObj;							/*Simply insert the object into the list at the beginning and return*/
		return 1;
	}
	SortedListPtr newObject = malloc(sizeof(struct SortedList));	/*Or else we need to insert it somewhere else, in which case we create a node to insert*/
	if(newObject == NULL)											/*If space not malloced successfully exit*/
		return 0;
	newObject->data = newObj;										/*Fill in the fields for the newObject pointer*/
	newObject->next = NULL;
	newObject->comparator = list->comparator;
	newObject->deleted = 1;
	while(temp != NULL){											/*While we have not reached the end of the list*/
		compval = list->comparator(temp->data, newObj);				/*the value holds the comparison value for the current node and the object to be inserted*/
		if(compval < 0){											/*If the node we are at is less than what we want to insert*/					
			if(prev == NULL){										/*And it is at the head of the list*/
				newObject->data = list->data;						/*Copy the information from the head to the object*/
				newObject->next = list->next;
				newObject->deleted = list->deleted;
				list->data = newObj;								/*Point the head to the new new object*/
				list->next = newObject;
				list->deleted = 1;
				return 1;
			}
			prev->next = newObject;									/*else it's somewhere in the middle of the list in which case just insert it there*/
			newObject->next = temp;
			return 1;
		}
		else if(compval == 0){										/*If we found something equivalent to what we want to insert*/
			newObject->next = temp->next;							/*Simply insert it before what we found*/
			temp->next = newObject;									
			return 1;
		}
		else{														/*Else we haven't found the right spot so keep looking*/		
			prev = temp;
			temp = temp->next;
		}
	}
	prev->next = newObject;											/*We have reached the end of the list so just insert at the end*/				
	return 1;

}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {		/*Creates an iterator struct and fills in the fields*/
	SortedListIteratorPtr iterator = malloc(sizeof(*iterator));
	if(iterator != NULL) {
		iterator->current = list;
		iterator->previous = NULL;
	}
	return iterator;
}

void SLDestroyIterator(SortedListIteratorPtr iter) {			/*frees the iterator pointer*/
	free(iter);
}

void *SLNextItem(SortedListIteratorPtr iter) {					/*Takes in an iterator pointer*/
	if(iter->current == NULL)									/*If the pointer is at the end of the list return null*/
		return NULL;											/*return null*/
	if(iter->previous != NULL) {								/*If the next item function has been run before*/
		while(iter->previous->next != iter->current) {			/*If we are at the correct spot via the current being immediately after the previous*/
			iter->previous = iter->previous->next;				/*move the previous pointer over*/
			if(iter->previous->deleted == 1) {					/*and if it is a deleted node*/
				iter->current = iter->previous;					/*move the current pointer back one*/
				break;
			}
		}
	}
	do {
		if(iter->current->deleted == 1)							/*if what we are pointing to has been deleted*/
			break;
		iter->current = iter->current->next;					/*move the current pointer over*/
	} while(iter->current != NULL);								/*while you haven't reached the end of the list*/
	void *object = NULL;										/*Create the data to be returned*/
	if(iter->current != NULL) {									/*if we haven't reached the end of the list*/
		object = iter->current->data;							/*obtain the data that is being pointed to*/
		iter->previous = iter->current;							/*move the pointers*/
		iter->current = iter->current->next;
	}
	return object;												/*return the data*/
}
