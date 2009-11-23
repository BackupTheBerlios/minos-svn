/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef MatchContactH
#define MatchContactH 
//---------------------------------------------------------------------------
class ListContact;
class ContactList;
class MatchContact
{
   public:
      void *treePointer;      // back pointer to object representing this contact on display
      // Not good if we want MVC - only one pointer per contact
      MatchContact( );
      virtual ~MatchContact();
      virtual ContactList *getContactList() const
      {
         return 0;
      }
      virtual ListContact *getListContact() const
      {
         return 0;
      }
      virtual BaseContestLog *getContactLog() const
      {
         return 0;
      }
      virtual BaseContact *getBaseContact() const
      {
         return 0;
      }
      virtual bool operator<( const MatchContact& rhs ) const = 0;
      virtual bool operator==( const MatchContact& rhs ) const = 0;
      virtual bool operator!=( const MatchContact& rhs ) const = 0;
      virtual void getText( std::string &dest, BaseContestLog *const ct ) const = 0;
};

class MatchListContact: public MatchContact
{
   public:
      ContactList *matchedList;
      ListContact *matchedListContact;
      MatchListContact( ContactList *ct, ListContact *lc );
      virtual ~MatchListContact();
      virtual ContactList *getContactList() const
      {
         return matchedList;
      }
      virtual ListContact *getListContact() const
      {
         return matchedListContact;
      }
      virtual bool operator<( const MatchContact& rhs ) const;
      virtual bool operator==( const MatchContact& rhs ) const;
      virtual bool operator!=( const MatchContact& rhs ) const;
      virtual void getText( std::string &dest, BaseContestLog *const ct ) const;
};
class MatchLogContact: public MatchContact
{
   public:
      BaseContestLog *matchedContest;
      BaseContact *matchedContact;
      MatchLogContact( BaseContestLog *ct, BaseContact *lc );
      virtual ~MatchLogContact();
      virtual BaseContestLog *getContactLog() const
      {
         return matchedContest;
      }
      virtual BaseContact *getBaseContact() const
      {
         return matchedContact;
      }
      virtual bool operator<( const MatchContact& rhs ) const;
      virtual bool operator==( const MatchContact& rhs ) const;
      virtual bool operator!=( const MatchContact& rhs ) const;
      virtual void getText( std::string &dest, BaseContestLog *const ct ) const;
};
#endif
