/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MjgTreeV.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
MjgNodeId::MjgNodeId(void)
{
}
MjgNodeId::~MjgNodeId()
{
}
//---------------------------------------------------------------------------
MjgTreeNode::MjgTreeNode(void):tnode(0), Child(0), Parent(0), PriorSibling(0),
NextSibling(0), ImageIndex(0), SelectedIndex(0), SortChildren(false), expanded(false),
NodeId(0)
{
}
MjgTreeNode::~MjgTreeNode(void)
{
	delete NodeId;
}
// return true if the nodes match
bool MjgTreeNode::Match(MjgNodeId *mi)
{
   if (NodeId == 0 || mi == 0)
      return false;

   return NodeId->Match(mi);
}
bool MjgTreeNode::Match(MjgTreeNode *m)
{
   if (m == 0 || NodeId == 0)
      return false;

   return NodeId->Match(m->NodeId);
}
bool MjgTreeNode::ExpandNode(void)
{
    if (expanded)
    	return true;

    expanded = true;
    return false;
}
bool MjgTreeNode::CollapseNode(void)
{
    return true;
}
bool MjgTreeNode::CanExpandNode(void)
{
	return true;
}
bool MjgTreeNode::CanCollapseNode(void)
{
	return true;
}
bool MjgTreeNode::CanEditNode(void)
{
	return true;
}
void MjgTreeNode::NodeEdited(void)
{}
bool MjgTreeNode::CanChangeNode(void)
{
	return true;
}
void MjgTreeNode::DelayChangeStart(void)	// start of delayed change
{}
void MjgTreeNode::DelayChangeEnd(void)	// end of delayed change
{}
void MjgTreeNode::NodeChanged(void)	// selection has changed to node
{}
void MjgTreeNode::DblClick(void)
{}
void MjgTreeNode::MouseDown(TMouseButton Button, TShiftState Shift)
{}
void MjgTreeNode::MouseUp(TMouseButton Button, TShiftState Shift)
{}
void MjgTreeNode::AddBefore(MjgTreeNode *base)
{
	// add "this" as prior sibling of base
   // NB that we may be adding a subtree
   if (Parent || PriorSibling || NextSibling)
   	ShowMessage(Name() + " is already in a structure!");
   if (base == this)
   	ShowMessage(AnsiString("You cannot chain ") + Name() + " to itself!");
	if (base)
   {
   	MjgTreeNode *temp = base->PriorSibling;
      base->PriorSibling = this;
      PriorSibling = temp;
      if (temp)
      {
      	temp->NextSibling = this;
      }
      NextSibling = base;

      if (base->Parent)
      {
	      Parent = base->Parent;
	      if (PriorSibling == 0)
	      	Parent->Child = this;
      }
   }
}
void MjgTreeNode::AddLastSibling(MjgTreeNode *base)
{
	// add "this" as last sibling of base
   // NB that we may be adding a subtree
   if (Parent || PriorSibling || NextSibling)
   	ShowMessage(Name() + " is already in a structure!");
   if (base == this)
   	ShowMessage(AnsiString("You cannot chain ") + Name() + " to itself!");
	if (base)
   {
   	MjgTreeNode *ls = base;
      while (ls && ls->NextSibling)
      	ls = ls->NextSibling;

      ls->NextSibling = this;
      NextSibling = 0;
      PriorSibling = ls;
      Parent = base->Parent;
   }
}
void MjgTreeNode::AddAfter(MjgTreeNode *base)
{
	// add "this" as next sibling of base
   // NB that we may be adding a subtree
   if (Parent || PriorSibling || NextSibling)
   	ShowMessage(Name() + " is already in a structure!");
   if (base == this)
   	ShowMessage(AnsiString("You cannot chain ") + Name() + " to itself!");
	if (base)
   {
   	MjgTreeNode *temp = base->NextSibling;
      base->NextSibling = this;
      NextSibling = temp;
      if (temp)
      {
      	temp->PriorSibling = this;
      }
      PriorSibling = base;
      Parent = base->Parent;
   }
}
void MjgTreeNode::AddAsChild(MjgTreeNode *base)
{
	// add "this" as (last)child of base
   // NB that we may be adding a subtree
   if (Parent || PriorSibling || NextSibling)
   	ShowMessage(Name() + " is already in a structure!");
   if (base == this)
   	ShowMessage(AnsiString("You cannot chain ") + Name() + " to itself!");

	if (base)
   {
   	MjgTreeNode *ls = base->Child;
      while (ls && ls->NextSibling)
      	ls = ls->NextSibling;

      if (ls)
      	AddAfter(ls);
      else
      {
      	base->Child = this;
         this->Parent = base;
         NextSibling = 0;
         PriorSibling = 0;
      }
   }
}
void MjgTreeNode::DeleteChildren(void)		// utility used in subtree walking and deleting
{
	MjgTreeNode *next = Child;
 	MjgTreeNode *temp = NextSibling;
   while (next)
   {
      next->DeleteChildren();

      // NB that the VCL structures have already gone...
      delete next;

      next = temp;
      if (next)
         temp = next->NextSibling;
   }
   Child = 0;
}
void MjgTreeNode::RemoveChildren()	// remove this nodes child tree.
                           // if "Clear" then recursively delete the node objects
{
   TTreeNode *ttn= GetVCLNode();
   if (ttn)
   {
   	// remove the subtree from the treeview
      // each node will call its matching NodeDeleted
      // which sets the VCL pointer to 0
      ttn->DeleteChildren();
   }
}

void MjgTreeNode::Remove()	// remove this node and child tree.
                           // if "Clear" then recursively delete the node objects
{
	// first we chain round this node
   MjgTreeNode *prior = PriorSibling;
   MjgTreeNode *next = NextSibling;

   if (Parent && Parent->Child == this)
		Parent->Child = next;

   Parent = 0;

   if (prior)
   	prior->NextSibling = next;

   if (next)
   	next->PriorSibling = prior;

  	// walk the subtree and delete all the nodes
   RemoveChildren();

   // the node will call its matching NodeDeleted
   // which sets the VCL pointer to 0
   TTreeNode *ttn= GetVCLNode();
   if (ttn)
	   ttn->Delete();

}
void MjgTreeNode::NodeDeleted(void)
{
	SetVCLNode(0);		// the VCL node has gone, but we haven't yet
}
//---------------------------------------------------------------------------
static inline MjgTreeView *ValidCtrCheck()
{
	return new MjgTreeView((TComponent *)0);
}
//---------------------------------------------------------------------------
__fastcall MjgTreeView::MjgTreeView(TComponent* Owner)
   : inherited(Owner),TempNode(0), InUpdate(false)
{
	DelayTimer = new TTimer(this);
   DelayTimer->Enabled = false;
   DelayTimer->OnTimer = DelayChange;
   DelayTimer->Interval = 200;
}
__fastcall MjgTreeView::MjgTreeView(HWND ParentWindow)
   : inherited(ParentWindow),TempNode(0), InUpdate(false)
{
	DelayTimer = new TTimer(this);
   DelayTimer->Enabled = false;
   DelayTimer->OnTimer = DelayChange;
   DelayTimer->Interval = 200;
}
__fastcall MjgTreeView::~MjgTreeView(void)
{ }
//---------------------------------------------------------------------------
namespace Mjgtreev
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(MjgTreeView)};
		RegisterComponents("MJGAddons", classes, 0);
	}
}
//---------------------------------------------------------------------------
void __fastcall MjgTreeView::DelayChange(System::TObject* Sender)
{
   DelayTimer->Enabled = false;

   if (Selected && !InUpdate)
   {
		MjgTreeNode *n = reinterpret_cast<MjgTreeNode *>(Selected->Data);
	   if (n)
	   {
	   	n->DelayChangeEnd();
	   }
	   else
	   {
	      if ( FOnDelayChangeEnd )
	         FOnDelayChangeEnd(Sender);
	   }
   }

}
//---------------------------------------------------------------------------
void __fastcall MjgTreeView::KillDelayTimer(void)
{
   DelayTimer->Enabled = false;
}
//---------------------------------------------------------------------------
bool __fastcall MjgTreeView::CanEdit(TTreeNode* Node)
{
	MjgTreeNode *n = reinterpret_cast<MjgTreeNode *>(Node->Data);
   if (n)
   {
   	return n->CanEditNode();
   }
   else
   {
		return inherited::CanEdit(Node);
   }
}
bool __fastcall MjgTreeView::CanChange(TTreeNode* Node)
{
	MjgTreeNode *n = reinterpret_cast<MjgTreeNode *>(Node->Data);
   if (n)
   {
   	return n->CanChangeNode();
   }
   else
   {
		return inherited::CanChange(Node);
   }
}
bool __fastcall MjgTreeView::CanCollapse(TTreeNode* Node)
{
	MjgTreeNode *n = reinterpret_cast<MjgTreeNode *>(Node->Data);
   if (n)
   {
   	return n->CanCollapseNode();
   }
   else
   {
		return inherited::CanCollapse(Node);
   }
}
bool __fastcall MjgTreeView::CanExpand(TTreeNode* Node)
{
	MjgTreeNode *n = reinterpret_cast<MjgTreeNode *>(Node->Data);
   if (n)
   {
   	return n->CanExpandNode();
   }
   else
   {
		return inherited::CanExpand(Node);
   }
}
void __fastcall MjgTreeView::Change(TTreeNode* Node)
{
	if ( !ComponentState.Contains(csLoading) )
	   DelayTimer->Enabled = false;
	MjgTreeNode *n = reinterpret_cast<MjgTreeNode *>(Node->Data);
   if (n)
   {
   	n->NodeChanged();
   }
   else
   {
		inherited::Change(Node);
   }
	if ( !ComponentState.Contains(csLoading) && (n || FOnDelayChangeStart) )
   {
   	if (n)
      	n->DelayChangeStart();
      else
	   	FOnDelayChangeStart(this,Node);
	   DelayTimer->Enabled = true;
   }
}
void __fastcall MjgTreeView::Collapse(TTreeNode* Node)
{
	inherited::Collapse(Node);
}
TTreeNode* __fastcall MjgTreeView::CreateNode(void)
{
	return inherited::CreateNode();
}
void __fastcall MjgTreeView::CreateParams(Controls::TCreateParams &Params)
{
	inherited::CreateParams(Params);
}
void __fastcall MjgTreeView::CreateWnd(void)
{
	inherited::CreateWnd();
}
void __fastcall MjgTreeView::DestroyWnd(void)
{
	inherited::DestroyWnd();
}
void __fastcall MjgTreeView::DoEndDrag(System::TObject* Target, int X, int Y)
{
	inherited::DoEndDrag(Target, X, Y);
}
void __fastcall MjgTreeView::DoStartDrag(Controls::TDragObject* &DragObject)
{
	inherited::DoStartDrag(DragObject);
}
//void __fastcall MjgTreeView::Edit(const TV_ITEMA &Item)
//{
//	inherited::Edit(Item);
//}
void __fastcall MjgTreeView::Expand(TTreeNode* Node)
{
	inherited::Expand(Node);
}
Controls::TDragImageList* __fastcall MjgTreeView::GetDragImages(void)
{
	return inherited::GetDragImages();
}
void __fastcall MjgTreeView::Loaded(void)
{
	inherited::Loaded();
}
void __fastcall MjgTreeView::Notification(Classes::TComponent* AComponent, Classes::TOperation Operation)
{
	inherited::Notification(AComponent, Operation);
}
void __fastcall MjgTreeView::SetDragMode(Controls::TDragMode Value)
{
	inherited::SetDragMode(Value);
}
bool __fastcall MjgTreeView::CustomDraw(const TRect &ARect, TCustomDrawStage Stage)
{
   return inherited::CustomDraw(ARect, Stage);
}
bool __fastcall MjgTreeView::CustomDrawItem(TTreeNode* Node, TCustomDrawState State, TCustomDrawStage Stage, bool &paint)
{
   return inherited::CustomDrawItem(Node, State, Stage, paint);
}
void __fastcall MjgTreeView::GetImageIndex(TTreeNode* Node)
{
   inherited::GetImageIndex(Node);
}
void __fastcall MjgTreeView::GetSelectedIndex(TTreeNode* Node)
{
   inherited::GetSelectedIndex(Node);
}
void __fastcall MjgTreeView::Delete(TTreeNode* Node)
{
   inherited::Delete(Node);
}
//---------------------------------------------------------------------------
MjgTreeNode * __fastcall MjgTreeView::GetNode(TTreeNode *t)
{
	if (!t)
   	return 0;
	return reinterpret_cast<MjgTreeNode *>(t->Data);
}
//---------------------------------------------------------------------------
void __fastcall MjgTreeView::UpdateState(bool state)
{
	if (state)
		Items->BeginUpdate();
    else
		Items->EndUpdate();
    FInUpdate = state;
}
//---------------------------------------------------------------------------
void __fastcall MjgTreeView::CreateWithChildren(MjgTreeNode *base)
{
   if (base)
   {
   	TTreeNode *ttn = base->GetVCLNode();
      if (!ttn)
      {
         ttn = Items->AddObject(0, base->Name(), base);
         base->SetVCLNode(ttn);

      }
      ttn->ImageIndex = base->ImageIndex;
      ttn->SelectedIndex = base->SelectedIndex;
   	if (base->Child)
		{
         base->GetVCLNode()->HasChildren = true;

         MjgTreeNode *n = base->Child;
         while (n)
         {
         	ttn = n->GetVCLNode();
            if (!ttn)
            {
               ttn = Items->AddChildObject(base->GetVCLNode(), n->Name(), n);
               n->SetVCLNode(ttn);
            }
            ttn->ImageIndex = n->ImageIndex;
            ttn->SelectedIndex = n->SelectedIndex;

            CreateWithChildren(n);

            n = n->NextSibling;
         }
         if (base->SortChildren)
            base->GetVCLNode()->AlphaSort();
      }

   }
}
void __fastcall MjgTreeView::CreateTree(MjgTreeNode *root, bool ExpandAll)
{
	// Replicate the node structure onto the tree

   UpdateState(true);
   Items->Clear();

   MjgTreeNode *n = root;
   while (n && n->PriorSibling)
   {
      if (n->PriorSibling == n)
      {
	   	AnsiString nam = n->Name();
         ShowMessage(nam);
      }
   	n = n->PriorSibling;
   }

   while (n)
   {
      CreateWithChildren(n);

      n = n->NextSibling;
   }
   if (ExpandAll)
   	FullExpand();
   UpdateState(false);
}

MjgTreeNode * __fastcall MjgTreeView::FindNode(MjgNodeId *mi)
{
	if (Items->Count)
    {
        TTreeNode *Noddy = Items->Item[0];
        while (Noddy)
        {
            MjgTreeNode *tn = GetNode(Noddy);
            if (tn)
            {
                if (tn->Match(mi))
                	return tn;
            }
            Noddy = Noddy->GetNext();
        }
    }
 	return 0;
}
MjgTreeNode * __fastcall MjgTreeView::FindNode(MjgTreeNode *mn)
{
	if (Items->Count)
    {
        TTreeNode *Noddy = Items->Item[0];
        while (Noddy)
        {
            MjgTreeNode *tn = GetNode(Noddy);
            if (tn)
            {
                if (tn->Match(mn))
                	return tn;
            }
            Noddy = Noddy->GetNext();
        }
    }
 	return 0;
}

MjgTreeNode * __fastcall MjgTreeView::FindNextNode(MjgTreeNode *t, MjgNodeId *mi)
{
	if (Items->Count)
    {
    	TTreeNode *Noddy = t->GetVCLNode();
        while (Noddy)
        {
            MjgTreeNode *tn = GetNode(Noddy);
            if (tn)
            {
                if (tn->Match(mi))
                	return tn;
            }
            Noddy = Noddy->GetNext();
        }
    }
 	return 0;
}



