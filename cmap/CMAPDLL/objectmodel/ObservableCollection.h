#pragma once
#ifndef _OBSERVABLECOLLECTION_H_
#define _OBSERVABLECOLLECTION_H_


#include "head.h"
#include "delegate.h"

enum NotifyCollectionChangedAction
{
	Add,
	Remove,
	Replace,
	Move,
	Reset
};

//ClassType 为item的类型，如maker，route......
template <typename ClassType>
class NotifyCollectionChangedEventArgs
{
public:
	NotifyCollectionChangedAction _action;
	vector<ClassType> _newItems;
	int _newStartingIndex;
	vector<ClassType> _oldItems;
	int _oldStartingIndex;

	void InitializeAdd(NotifyCollectionChangedAction action, vector<ClassType> newItems, int newStartingIndex);

	void InitializeRemove(NotifyCollectionChangedAction action, vector<ClassType> oldItems, int oldStartingIndex);

	void InitializeAddOrRemove(NotifyCollectionChangedAction action, vector<ClassType> changedItems, int startingIndex);

	void InitializeMoveOrReplace(NotifyCollectionChangedAction action, vector<ClassType> newItems, vector<ClassType> oldItems, int startingIndex, int oldStartingIndex);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> changedItems);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType changedItem);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> newItems, vector<ClassType> oldItems);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> changedItems, int startingIndex);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType changedItem, int index);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType newItem, ClassType oldItem);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> newItems, vector<ClassType> oldItems, int startingIndex);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> changedItems, int index, int oldIndex);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType changedItem, int index, int oldIndex);

	NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType newItem, ClassType oldItem, int index);

	NotifyCollectionChangedAction ActionGet();

	vector<ClassType> NewItemsGet();

	int NewStartingIndexGet();

	vector<ClassType> OldItemsGet();

	int OldStartingIndexGet();

};

class EXPORTS_CLASS SimpleMonitor
{
	static int _busyCount;
public:
	void Enter();


	void Out();


	bool Busy();

};


//ClassType 实例化时，使用的类类型比如，MapMaker,MapRoute......
template <typename ClassType>
class ObservableCollection
{
public:
	ObservableCollection(void);
	ObservableCollection(vector<ClassType> collection);
	~ObservableCollection(void);

	SimpleMonitor _monitor;
	string CountString ;
	string IndexerName ;
	vector<ClassType> Items;

	CDelegate<ObservableCollection*, NotifyCollectionChangedEventArgs<ClassType>> CollectionChanged;

	void CopyFrom(vector<ClassType> collection);

	void inBlockReentrancy();

	void outBlockReentrancy();

	bool CheckReentrancy();

	virtual void OnCollectionChanged(NotifyCollectionChangedEventArgs<ClassType> e);

	void OnCollectionChanged(NotifyCollectionChangedAction action, ClassType item, int index);

	void OnCollectionChanged(NotifyCollectionChangedAction action, ClassType item, int index, int oldIndex);

	void OnCollectionChanged(NotifyCollectionChangedAction action, ClassType oldItem, ClassType newItem, int index);

	void OnCollectionReset();

	void ClearItems();

	void InsertItem(int index, ClassType item);

	void MoveItem(int oldIndex, int newIndex);

	void Move(int oldIndex, int newIndex);

	void RemoveItem(int index);

	void Remove(ClassType item);

	void SetItem(int index, ClassType item);

	void AddItem(ClassType item);

	bool Contains(ClassType item);
};



////////////
////NotifyCollectionChangedEventArgs
///////////
template <typename ClassType>
void NotifyCollectionChangedEventArgs<ClassType>::InitializeAdd(NotifyCollectionChangedAction action, vector<ClassType> newItems, int newStartingIndex)
{
	this->_action = action;
	if (newItems.size() > 0)
	{
		copy(newItems.begin(), newItems.end(), back_inserter(_newItems));
	}
	this->_newStartingIndex = newStartingIndex;
}

template <typename ClassType>
void NotifyCollectionChangedEventArgs<ClassType>::InitializeRemove(NotifyCollectionChangedAction action, vector<ClassType> oldItems, int oldStartingIndex)
{
	this->_action = action;
	if (oldItems.size() > 0)
	{
		copy(oldItems.begin(), oldItems.end(), back_inserter(_oldItems));
	}
	this->_oldStartingIndex = oldStartingIndex;
}

template <typename ClassType>
void NotifyCollectionChangedEventArgs<ClassType>::InitializeAddOrRemove(NotifyCollectionChangedAction action, vector<ClassType> changedItems, int startingIndex)
{
	if(action == NotifyCollectionChangedAction::Add)
	{
		this->InitializeAdd(action, changedItems, startingIndex);
	}
	else if(action == NotifyCollectionChangedAction::Remove)
	{
		this->InitializeRemove(action, changedItems, startingIndex);
	}
	else
	{
		printf("InvariantFailure, Unsupported action: {%d}\n", action);
	}
}

template <typename ClassType>
void NotifyCollectionChangedEventArgs<ClassType>::InitializeMoveOrReplace(NotifyCollectionChangedAction action, vector<ClassType> newItems, vector<ClassType> oldItems, int startingIndex, int oldStartingIndex)
{
	this->InitializeAdd(action, newItems, startingIndex);
	this->InitializeRemove(action, oldItems, oldStartingIndex);
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(action != NotifyCollectionChangedAction::Reset)
	{
		printf("WrongActionForCtor: action\n");
	}
	vector<ClassType> emptyItems;
	this->InitializeAdd(action, emptyItems, -1);
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> changedItems)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(((action != NotifyCollectionChangedAction::Add) && (action != NotifyCollectionChangedAction::Remove)) && (action != NotifyCollectionChangedAction::Reset))
	{
		printf("MustBeResetAddOrRemoveActionForCtor: action\n");
	}
	if(action == NotifyCollectionChangedAction::Reset)
	{
		if(changedItems.size() != 0)
		{
			printf("ResetActionRequiresNullItem: action\n");
		}
		this->InitializeAdd(action, changedItems, -1);
	}
	else
	{
		if(changedItems.size() == 0 )
		{
			printf("changedItems\n");
		}
		this->InitializeAddOrRemove(action, changedItems, -1);
	}
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType changedItem)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(((action != NotifyCollectionChangedAction::Add) && (action != NotifyCollectionChangedAction::Remove)) && (action != NotifyCollectionChangedAction::Reset))
	{
		printf("MustBeResetAddOrRemoveActionForCtor: action\n");
	}
	if(action == NotifyCollectionChangedAction::Reset)
	{
		if( !changedItem->isEmpty() )
		{
			printf("ResetActionRequiresNullItem: action\n");
		}
		vector<ClassType> empty;
		this->InitializeAdd(action, empty, -1);
	}
	else
	{
		vector<ClassType> listmp(1, changedItem);
		this->InitializeAddOrRemove(action, listmp, -1);
	}
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> newItems, vector<ClassType> oldItems)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(action != NotifyCollectionChangedAction::Replace)
	{
		printf("MustBeResetAddOrRemoveActionForCtor: action\n");
	}
	if(newItems.size() == 0)
	{
		printf("newItems\n");
	}
	if(oldItems.size() == 0)
	{
		printf("oldItems\n");
	}
	this->InitializeMoveOrReplace(action, newItems, oldItems, -1, -1);
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> changedItems, int startingIndex)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(((action != NotifyCollectionChangedAction::Add) && (action != NotifyCollectionChangedAction::Remove)) && (action != NotifyCollectionChangedAction::Reset))
	{
		printf("MustBeResetAddOrRemoveActionForCtor: action\n");
	}
	if(action == NotifyCollectionChangedAction::Reset)
	{
		if(changedItems.size() != 0)
		{
			printf("ResetActionRequiresNullItem: action\n");
		}
		if(startingIndex != -1)
		{
			printf("ResetActionRequiresIndexMinus1: \naction");
		}
		vector<ClassType> empty;
		this->InitializeAdd(action, empty, -1);
	}
	else
	{
		if(changedItems.size() == 0)
		{
			printf("changedItems\n");
		}
		if(startingIndex < -1)
		{
			printf("IndexCannotBeNegative: startingIndex\n");
		}
		this->InitializeAddOrRemove(action, changedItems, startingIndex);
	}
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType changedItem, int index)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(((action != NotifyCollectionChangedAction::Add) && (action != NotifyCollectionChangedAction::Remove)) && (action != NotifyCollectionChangedAction::Reset))
	{
		printf("MustBeResetAddOrRemoveActionForCtor: action\n");
	}
	if(action == NotifyCollectionChangedAction::Reset)
	{
		if(!changedItem->isEmpty())
		{
			printf("ResetActionRequiresNullItem: action\n");
		}
		if(index != -1)
		{
			printf("ResetActionRequiresIndexMinus1: action\n");
		}
		vector<ClassType> empty;
		this->InitializeAdd(action, empty, -1);
	}
	else
	{
		vector<ClassType> listmp( 1, changedItem);
		this->InitializeAddOrRemove(action, listmp, index);
	}
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType newItem, ClassType oldItem)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(action != NotifyCollectionChangedAction::Replace)
	{
		printf("WrongActionForCtor: action\n");
	}
	vector<ClassType> objectnew( 1, newItem);
	vector<ClassType> objectold( 1, oldItem);
	this->InitializeMoveOrReplace(action,  objectnew, objectold, -1, -1);
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> newItems, vector<ClassType> oldItems, int startingIndex)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(action != NotifyCollectionChangedAction::Replace)
	{
		printf("WrongActionForCtor: action\n");
	}
	if(newItems.size() == 0)
	{
		printf("newItems\n");
	}
	if(oldItems.size() == 0)
	{
		printf("oldItems\n");
	}
	this->InitializeMoveOrReplace(action, newItems, oldItems, startingIndex, startingIndex);
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, vector<ClassType> changedItems, int index, int oldIndex)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(action != NotifyCollectionChangedAction::Move)
	{
		printf("WrongActionForCtor: action\n");
	}
	if(index < 0)
	{
		printf("IndexCannotBeNegative: index\n");
	}
	this->InitializeMoveOrReplace(action, changedItems, changedItems, index, oldIndex);
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType changedItem, int index, int oldIndex)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(action != NotifyCollectionChangedAction::Move)
	{
		printf("WrongActionForCtor: action\n");
	}
	if(index < 0)
	{
		printf("IndexCannotBeNegative: index\n");
	}
	vector<ClassType> newItems(1, changedItem);
	this->InitializeMoveOrReplace(action, newItems, newItems, index, oldIndex);
}

template <typename ClassType>
NotifyCollectionChangedEventArgs<ClassType>::NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction action, ClassType newItem, ClassType oldItem, int index)
{
	this->_newStartingIndex = -1;
	this->_oldStartingIndex = -1;
	if(action != NotifyCollectionChangedAction::Replace)
	{
		printf("WrongActionForCtor: action\n");
	}
	vector<ClassType> object1(1, newItem);
	vector<ClassType> object2(1, oldItem);
	this->InitializeMoveOrReplace(action, object1, object2, index, index);
}

template <typename ClassType>
NotifyCollectionChangedAction NotifyCollectionChangedEventArgs<ClassType>::ActionGet()
{
	return this->_action;
}

template <typename ClassType>
vector<ClassType> NotifyCollectionChangedEventArgs<ClassType>::NewItemsGet()
{
	return this->_newItems;
}

template <typename ClassType>
int NotifyCollectionChangedEventArgs<ClassType>::NewStartingIndexGet()
{
	return this->_newStartingIndex;
}

template <typename ClassType>
vector<ClassType> NotifyCollectionChangedEventArgs<ClassType>::OldItemsGet()
{
	return this->_oldItems;
}

template <typename ClassType>
int NotifyCollectionChangedEventArgs<ClassType>::OldStartingIndexGet()
{
	return this->_oldStartingIndex;
}

////////////
////////////
////////////
////////////
////////////
////////////
////ObservableCollection
////////////
////////////
////////////
////////////
////////////
////////////
template <typename ClassType>
ObservableCollection<ClassType>::ObservableCollection(void)
{
	CountString = "Count";
	IndexerName = "Item[]";
}

template <typename ClassType>
ObservableCollection<ClassType>::ObservableCollection(vector<ClassType> collection)
{
	CountString = "Count";
	IndexerName = "Item[]";
	this->CopyFrom(collection);
}

template <typename ClassType>
ObservableCollection<ClassType>::~ObservableCollection(void)
{
	vector<ClassType>::iterator iter = Items.begin();
	while(iter != Items.end())
	{
		delete *iter;
		iter++;
	}
	Items.clear();
}

template <typename ClassType>
void ObservableCollection<ClassType>::CopyFrom(vector<ClassType> collection)
{
	if( collection.size() != 0 )
	{
		copy(collection.begin(), collection.end(), back_inserter(Items));
	}
}

template <typename ClassType>
void ObservableCollection<ClassType>::inBlockReentrancy()
{
	this->_monitor.Enter();
}

template <typename ClassType>
void ObservableCollection<ClassType>::outBlockReentrancy()
{
	this->_monitor.Out();
}

template <typename ClassType>
bool ObservableCollection<ClassType>::CheckReentrancy()
{
	if (this->_monitor.Busy() && this->CollectionChanged.DelegateSize() > 1)
	{
		printf("ObservableCollectionReentrancyNotAllowed\n");
		return false;
	}
	return true;
}

template <typename ClassType>
void ObservableCollection<ClassType>::OnCollectionChanged(NotifyCollectionChangedEventArgs<ClassType> e)
{
	this->inBlockReentrancy();
	this->CollectionChanged.DelegateDo(this, e);
	this->outBlockReentrancy();
}

template <typename ClassType>
void ObservableCollection<ClassType>::OnCollectionChanged(NotifyCollectionChangedAction action, ClassType item, int index)
{
	this->OnCollectionChanged(NotifyCollectionChangedEventArgs<ClassType>(action, item, index));
}

template <typename ClassType>
void ObservableCollection<ClassType>::OnCollectionChanged(NotifyCollectionChangedAction action, ClassType item, int index, int oldIndex)
{
	this->OnCollectionChanged(NotifyCollectionChangedEventArgs<ClassType>(action, item, index, oldIndex));
}

template <typename ClassType>
void ObservableCollection<ClassType>::OnCollectionChanged(NotifyCollectionChangedAction action, ClassType oldItem, ClassType newItem, int index)
{
	this->OnCollectionChanged(NotifyCollectionChangedEventArgs<ClassType>(action, newItem, oldItem, index));
}

template <typename ClassType>
void ObservableCollection<ClassType>::OnCollectionReset()
{
	this->OnCollectionChanged(NotifyCollectionChangedEventArgs<ClassType>(NotifyCollectionChangedAction::Reset));
}

template <typename ClassType>
void ObservableCollection<ClassType>::ClearItems()
{
	if (this->CheckReentrancy())
	{
		vector<ClassType>::iterator iter = Items.begin();
		while(iter != Items.end())
		{
			delete *iter;
			iter++;
		}
		Items.clear();
		this->OnCollectionReset();
	}
}

template <typename ClassType>
void ObservableCollection<ClassType>::InsertItem(int index, ClassType item)
{
	if (this->CheckReentrancy())
	{
		Items.insert(Items.begin() + index, item);
		this->OnCollectionChanged(NotifyCollectionChangedAction::Add, item, index);
	}
}

template <typename ClassType>
void ObservableCollection<ClassType>::MoveItem(int oldIndex, int newIndex)
{
	if (this->CheckReentrancy())
	{
		if (Items.size() >= oldIndex)
		{
			ClassType item = Items.at(oldIndex);
			Items.erase(Items.begin() + oldIndex);
			InsertItem(newIndex, item);
			this->OnCollectionChanged(NotifyCollectionChangedAction::Move, item, newIndex, oldIndex);
		}
	}
}

template <typename ClassType>
void ObservableCollection<ClassType>::Move(int oldIndex, int newIndex)
{
	this->MoveItem(oldIndex, newIndex);
}

template <typename ClassType>
void ObservableCollection<ClassType>::RemoveItem(int index)
{
	if (this->CheckReentrancy())
	{
		if (Items.size() >= index)
		{
			ClassType item = Items.at(index);
			Items.erase(Items.begin() + index);
			this->OnCollectionChanged(NotifyCollectionChangedAction::Remove, item, index);
			delete item;
		}
	}
}

template <typename ClassType>
void ObservableCollection<ClassType>::SetItem(int index, ClassType item)
{
	if (this->CheckReentrancy())
	{
		if (Items.size() >= index)
		{
			ClassType oldItem = Items.at(index);
			Items.erase(Items.begin() + index);
			InsertItem(index, item);
			this->OnCollectionChanged(NotifyCollectionChangedAction::Replace, oldItem, item, index);
			delete oldItem;
		}
	}
}
template <typename ClassType>
void ObservableCollection<ClassType>::AddItem(ClassType item)
{
	if (this->CheckReentrancy())
	{
		Items.push_back(item);
		int index = Items.size() - 1;
		this->OnCollectionChanged(NotifyCollectionChangedAction::Add, item, index);
	}
}

template <typename ClassType>
bool ObservableCollection<ClassType>::Contains(ClassType item)
{
	vector<ClassType>::iterator iter;
	iter = Items.begin();
	while( iter != Items.end())
	{
		if ( (*iter) == item)
		{
			return true;
		}
		iter++;
	}
	return false;
}

template <typename ClassType>
void ObservableCollection<ClassType>::Remove(ClassType item)
{
	vector<ClassType>::iterator iter;
	iter = Items.begin();
	int index = 0;
	while( iter != Items.end())
	{
		if ( (*iter) == item)
		{
			Items.erase(iter);
			this->OnCollectionChanged(NotifyCollectionChangedAction::Remove, item, index);
			return ;
		}
		iter++;
		index++;
	}
}

#endif

