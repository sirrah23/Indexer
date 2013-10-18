#include "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf) {
	SortedListPtr list = malloc(sizeof(struct SortedList)); /*malloce space for the list*/
	if(list != NULL) {										/*if the list was created successfully*/
		list->data = NULL;									/*fill in it's default fields*/
		list->next = NULL; 
		list->files = NULL;									/*a sorted list for the files for each word*/
		list->comparator = cf;
	}
	return list;											/*return the list*/
}

SortedList2Ptr SLCreate2(int(*cf)(int,int)) {
    SortedList2Ptr list = malloc(sizeof(struct SortedList2));
    if(list != NULL) {
        list->data = NULL;
        list->next = NULL;
        list->comparator = cf;
        list->count = 0;
    }
    return list;
}

void SLDestroy(SortedListPtr list) {
	SortedListPtr temp, temp2;	/*Pointers used to parse through the list*/
	temp = temp2 = list;		/*Point the pointers to the start*/
	while(temp != NULL){		/*While you haven't reached the end of the list*/
		temp2 = temp->next;		/*Move the other pointer to the next node*/
		if(temp2 == NULL) {     /*if next node doesn't exist that means you're at the end*/
            free(temp->data);
            SLDestroy2(temp->files);
			free(temp);			/*Free that node*/
			break;
		}
		else{					/*else the list still has more stuff*/
            free(temp->data);
            SLDestroy2(temp->files);
			free(temp);			/*Free the node itself*/
			temp = temp2;		/*Move onto the next node in the list*/
		}
	}	
}

void SLDestroy2(SortedList2Ptr list) {
    SortedList2Ptr temp, temp2;  /*Pointers used to parse through the list*/
    temp = temp2 = list;        /*Point the pointers to the start*/
    while(temp != NULL){        /*While you haven't reached the end of the list*/
        temp2 = temp->next;     /*Move the other pointer to the next node*/
        if(temp2 == NULL){      /*if next node doesn't exist that means you're at the end*/
            free(temp->data);
            free(temp);         /*Free that node*/
            break;
        }
        else{                   /*else the list still has more stuff*/
            free(temp->data);
            free(temp);         /*Free the node itself*/
            temp = temp2;       /*Move onto the next node in the list*/
        }
    }
}

int SLInsert2(SortedList2Ptr list, void *newObj, CompareFuncT comp) {
    SortedList2Ptr prev = NULL;                          /*point a previous pointer to null*/
    SortedList2Ptr temp = list;                          /*point a temp pointer to the beginning of the list*/
    int compval;                                        /*variable that will hold the compare function's return*/
    if(list->data == NULL){                             /*If the list is empty*/
        list->data = newObj;                            /*Simply insert the object into the list at the beginning and return*/
        list->count++;
        return 1;
    }
    while(temp != NULL) {
        compval = comp(temp->data, newObj);
        if(compval == 0) { /*loops through the list until a match is found*/
            temp->count++; /*increment word occurrence in that match*/
            if(prev != NULL) { /*sort the list if the match found is not the first one in the list*/
                SortedList2Ptr prev2 = NULL;
                SortedList2Ptr temp2 = list; /*point to the beginning of the list*/
                int compval2;
                while(temp2 != temp) { /*loops through the list until the correct position is found*/
                    compval2 = temp->comparator(temp2->count, temp->count);
                    if(compval2 < 0) { /*position found*/
                        prev->next = temp->next; /*disconnect temp from the list*/
                        if(prev2 == NULL) { /*if temp2 is in front of the list*/
                            void *x = temp2->data; /*swaps data between the node (temp2) in front of the list and temp*/
                            int y = temp2->count;
                            temp2->data = temp->data;
                            temp2->count = temp->count;
                            temp->data = x; temp->count = y;
                            temp->next = temp2->next; /*puts temp after temp2*/
                            temp2->next = temp;
                        } else {
                            temp->next = prev2->next; /*puts temp between prev2 and prev2->next*/
                            prev2->next = temp;
                        }
                        break; /*done sorting*/
                    }
                    prev2 = temp2; /*iterate to the next node*/
                    temp2 = temp2->next;
                }
            }
            return 1; /*return if done with the match found*/
        }
        prev = temp; /*continue on until a match is found or not found*/
        temp = temp->next;
    }
    /*if no match is found create a new node*/
    SortedList2Ptr newObject = malloc(sizeof(struct SortedList2));
    if(newObject == NULL)
        return 0;
    newObject->data = newObj;
    newObject->count = 1;
    newObject->next = NULL;
    newObject->comparator = list->comparator;
    prev->next = newObject; /*put it at the end of the list*/
    return 1;
}

int SLInsert(SortedListPtr list, void *newObj, void *newObj2) {		/*takes in a list and void pointer*/
	SortedListPtr prev = NULL;							/*point a previous pointer to null*/
	SortedListPtr temp = list;							/*point a temp pointer to the beginning of the list*/
	int compval;										/*variable that will hold the compare function's return*/
	if(list->data == NULL){								/*If the list is empty*/
		list->data = newObj;                            /*Simply insert the object into the list at the beginning and return*/
        if(SLInsert2(list->files, newObj2, list->comparator))
		    return 1;
        else
            return 0;
	}
	SortedListPtr newObject = malloc(sizeof(struct SortedList));	/*Or else we need to insert it somewhere else, in which case we create a node to insert*/
	if(newObject == NULL)											/*If space not malloced successfully exit*/
		return 0;
	newObject->data = newObj;										/*Fill in the fields for the newObject pointer*/
	newObject->files = NULL;
    newObject->next = NULL;
	newObject->comparator = list->comparator;
	while(temp != NULL){											/*While we have not reached the end of the list*/
		compval = list->comparator(temp->data, newObj);				/*the value holds the comparison value for the current node and the object to be inserted*/
		if(compval < 0){											/*If the node we are at is less than what we want to insert*/					
			if(prev == NULL){										/*And it is at the head of the list*/
				newObject->data = list->data;                       /*Copy the information from the head to the object*/
				newObject->files = list->files;
                newObject->next = list->next;
                list->data = newObj;								/*Point the head to the new new object*/
				list->next = newObject;
                list->files = SLCreate2(newObject->files->comparator);
                if(list->files == NULL)
                    return 0;
                if(SLInsert2(list->files, newObj2, list->comparator))
				    return 1;
                else
                    return 0;
			}
			prev->next = newObject;									/*else it's somewhere in the middle of the list in which case just insert it there*/
			newObject->next = temp;
            newObject->files = SLCreate2(prev->files->comparator);
            if(newObject->files == NULL)
                return 0;
            if(SLInsert2(newObject->files, newObj2, newObject->comparator))
			    return 1;
            else
                return 0;
		}
		else if(compval == 0){										/*If we found something equivalent to what we want to insert*/
			if(SLInsert2(temp->files, newObj2, temp->comparator))
			    return 1;
            else
                return 0;
		}
		else{														/*Else we haven't found the right spot so keep looking*/		
			prev = temp;
			temp = temp->next;
		}
	}
	prev->next = newObject;                                         /*We have reached the end of the list so just insert at the end*/				
    newObject->files = SLCreate2(prev->files->comparator);
    if(newObject->files == NULL)
        return 0;
    if(SLInsert2(newObject->files, newObj2, newObject->comparator))
	    return 1;
    else
        return 0;
}
