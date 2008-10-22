/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#ifndef MjgTreeVH
#define MjgTreeVH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
//---------------------------------------------------------------------------
// this is a supporting class to represent a node in the tree structure
class PACKAGE MjgNodeId
{
	public:
   	MjgNodeId(void);
   	virtual ~MjgNodeId();
		//-----------------------------------------------------------------------
      // return true if the nodes match
      virtual bool Match(MjgNodeId *m) = 0;
};

class PACKAGE MjgTreeNode
{
	private:
      bool expanded;
      void DeleteChildren(void);		// utility used in subtree walking and deleting
		TTreeNode *tnode;

	public:
		MjgNodeId *NodeId;

		//-----------------------------------------------------------------------
      // clones of the properties desired for the VCL TTreeNode
      int ImageIndex;
      int SelectedIndex;

      // set if children should be sorted when the tree is constructed
      bool SortChildren;
      bool ExpandOnCreate;

		//-----------------------------------------------------------------------
      // chain links to form the tree structure
   	MjgTreeNode *Child;
   	MjgTreeNode *Parent;
      MjgTreeNode *PriorSibling;
      MjgTreeNode *NextSibling;
      // we will want "immediate" and "delayed" versions of these
      // or some way of doing a tree compare and re-create
      // ? do we need a "shallow clone" to produce a similar tree structure
      //
      virtual void AddLastSibling(MjgTreeNode *);
      virtual void AddBefore(MjgTreeNode *);
      virtual void AddAfter(MjgTreeNode *);
      virtual void AddAsChild(MjgTreeNode *);

      // And we still need... (also in immediate and delayed forms)

      void Remove();	// remove this node and child tree.
      									// Recursively delete the node objects

      void RemoveChildren();	// remove this nodes child tree (but not this node).
      									//Recursively delete the node objects
      // ExpandNode returns true if the node is already expanded, or expands
      // the node. It recurses until a leaf returns false, implying that it
      // cannot expand further
      virtual bool ExpandNode(void);

      // CollapseNode is used when collapsing; it would give the option of
      // getting rid of data structures under collapsed nodes
      virtual bool CollapseNode(void);
		//-----------------------------------------------------------------------
	   MjgTreeNode(void);
	   virtual ~MjgTreeNode(void);
		virtual bool Match(MjgTreeNode *m);
		virtual bool Match(MjgNodeId *mi);

		//-----------------------------------------------------------------------
      // linkage with TTreeNode
      TTreeNode *GetVCLNode(void){return tnode;}
      void SetVCLNode(TTreeNode *n){tnode = n;}    // should this have been a property?
      // boilerplate needed in treview OnDeletion:
		//	MjgTreeNode *n = ReportTree->GetNode(Node);
		//   if (n)
		//   	n->NodeDeleted();
		virtual void NodeDeleted(void);

      // get the name of the node - used to give string for display
		virtual UnicodeString Name(void) = 0;

		//-----------------------------------------------------------------------
      // if node is expandable return true
      virtual bool CanExpandNode(void);
      // if node is collapsable return true
      virtual bool CanCollapseNode(void);
      // if node is changeable to return true
      virtual bool CanChangeNode(void);
      // if node is editable return true
      virtual bool CanEditNode(void);
      virtual void NodeEdited(void); 	// edit has completed

      virtual void DelayChangeStart(void);	// start of delayed change
      virtual void DelayChangeEnd(void);	// end of delayed change
      virtual void NodeChanged(void);	// selection has changed to node

		// DblClick comes from TTreeNodes, not the tree, which makes it
      // dificult to intercept. A small bit of boilerplate
      // in the form has to be used instead:

      //   TTreeNode *ttn = TreeView1->Selected;
      //   if (ttn)
      //      TreeView1->GetNode(ttn)->DblClick();
      virtual void DblClick(void);

      // Mouse down and up are currently boilerplated, but may be possible
      // to get into treeview
      virtual void MouseDown(TMouseButton Button, TShiftState Shift);
      virtual void MouseUp(TMouseButton Button, TShiftState Shift);

};
//---------------------------------------------------------------------------
class PACKAGE MjgTreeView : public TCustomTreeView
{
	// NB Properties CanExpand, CanCollapse, CanEdit, CanChange
   // are pre-empted by MjgTreeNode, and the relevant method is
   // called on the node.
__published:
	__property Align ;
	__property Anchors ;
	__property AutoExpand ;
	__property BiDiMode ;
	__property BorderStyle ;
	__property BorderWidth ;
	__property ChangeDelay ;
	__property Color ;
	__property Ctl3D ;
	__property Constraints ;
	__property DragKind ;
	__property DragCursor ;
	__property DragMode ;
	__property Enabled ;
	__property Font ;
	__property HideSelection ;
	__property HotTrack ;
	__property Images ;
	__property Indent ;
	__property Items ;
	__property ParentBiDiMode ;
	__property ParentColor ;
	__property ParentCtl3D ;
	__property ParentFont ;
	__property ParentShowHint ;
	__property PopupMenu ;
	__property ReadOnly ;
	__property RightClickSelect ;
	__property RowSelect ;
	__property ShowButtons ;
	__property ShowHint ;
	__property ShowLines ;
	__property ShowRoot ;
	__property SortType ;
	__property StateImages ;
	__property TabOrder ;
	__property TabStop ;
	__property ToolTips ;
	__property Visible ;
	__property OnChange ;
	__property OnChanging ;
	__property OnClick ;
	__property OnCollapsing ;
	__property OnCollapsed ;
	__property OnCompare ;
	__property OnCustomDraw ;
	__property OnCustomDrawItem ;
	__property OnDblClick ;
	__property OnDeletion ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEdited ;
	__property OnEditing ;
	__property OnEndDock ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnExpanding ;
	__property OnExpanded ;
	__property OnGetImageIndex ;
	__property OnGetSelectedIndex ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnStartDock ;
	__property OnStartDrag ;
__published:
	// our own extra properties
   __property TTVChangedEvent OnDelayChangeStart = { read = FOnDelayChangeStart, write = FOnDelayChangeStart };
   __property TNotifyEvent OnDelayChangeEnd = { read = FOnDelayChangeEnd, write = FOnDelayChangeEnd };
   __property bool InUpdate = { read = FInUpdate, write = UpdateState };

public:
	/* TCustomTreeView.Create */ __fastcall virtual MjgTreeView(Classes::TComponent* AOwner);
	/* TWinControl.CreateParented */ __fastcall MjgTreeView(HWND ParentWindow);
	/* TCustomTreeView.Destroy */ __fastcall virtual ~MjgTreeView(void);

	virtual void __fastcall KillDelayTimer(void);

   // re-walk the tree from a base node, add in situ. If base is not displayed
   // it will be be displayed at root level
	void __fastcall CreateWithChildren(MjgTreeNode *base);
   void __fastcall CreateTree(MjgTreeNode *root, bool ExpandAll);
   MjgTreeNode * __fastcall GetNode(TTreeNode *t);
	MjgTreeNode * __fastcall FindNode(MjgNodeId *mi);
	MjgTreeNode * __fastcall FindNode(MjgTreeNode *mn);
	MjgTreeNode * __fastcall FindNextNode(MjgTreeNode *t, MjgNodeId *mi);

private:
   typedef Comctrls::TCustomTreeView inherited;

   TTreeNode * TempNode;
   // -----------------------------------------------------------------------
   // timer for use with delayed changes
	TTimer    * DelayTimer;

   TTVChangedEvent FOnDelayChangeStart;
   TNotifyEvent    FOnDelayChangeEnd;
	void virtual __fastcall DelayChange(System::TObject* Sender);
   void __fastcall UpdateState(bool);

   bool FInUpdate;
protected:
	DYNAMIC bool __fastcall CanEdit(TTreeNode* Node);
	DYNAMIC bool __fastcall CanChange(TTreeNode* Node);
	DYNAMIC bool __fastcall CanCollapse(TTreeNode* Node);
	DYNAMIC bool __fastcall CanExpand(TTreeNode* Node);
	DYNAMIC void __fastcall Change(TTreeNode* Node);
	DYNAMIC void __fastcall Collapse(TTreeNode* Node);
	virtual TTreeNode* __fastcall CreateNode(void);
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	virtual bool __fastcall CustomDraw(const TRect &ARect, TCustomDrawStage Stage);
	virtual bool __fastcall CustomDrawItem(TTreeNode* Node, TCustomDrawState State, TCustomDrawStage Stage, bool &);
	virtual void __fastcall DestroyWnd(void);
	DYNAMIC void __fastcall Delete(TTreeNode* Node);
   DYNAMIC void __fastcall DoEndDrag(System::TObject* Target, int X, int Y);
	DYNAMIC void __fastcall DoStartDrag(Controls::TDragObject* &DragObject);
//	DYNAMIC void __fastcall Edit(const tagTVITEMA &Item);
	DYNAMIC void __fastcall Expand(TTreeNode* Node);
	virtual Controls::TDragImageList* __fastcall GetDragImages(void);
	virtual void __fastcall GetImageIndex(TTreeNode* Node);
	virtual void __fastcall GetSelectedIndex(TTreeNode* Node);
//   bool __fastcall IsCustomDrawn(TCustomDrawTarget Target, TCustomDrawStage Stage);
	virtual void __fastcall Loaded(void);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation
		);
	virtual void __fastcall SetDragMode(Controls::TDragMode Value);
};
//---------------------------------------------------------------------------
#endif
