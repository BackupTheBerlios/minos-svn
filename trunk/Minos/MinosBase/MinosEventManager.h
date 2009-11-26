/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef MinosEventManagerH
#define MinosEventManagerH
//---------------------------------------------------------------------------
#include <list>
#include "MinosEventData.h"
//---------------------------------------------------------------------------
// Sample use
//
//class x
//{
//	MinosEventListener  EL_StatusText;
//	void  SetStatusText_Event ( MinosEventBase & Event );
//
//	x() :
//	EL_StatusText ( EN_StatusText, & SetStatusText_Event )
//	{ }
//
//};
//
//void  x::SetStatusText_Event ( MinosEventBase & Event )
//{
//	if ( Event.EventNo == EN_StatusText )
//	{
//		ActionEvent<String, EN_StatusText> & S = dynamic_cast<ActionEvent<String, EN_StatusText> &> ( Event );
//		StatusPanel1->SimpleText = S.Data;
//	}
//}
//void xx()
//{
//	String s = "Text for status bar";
//	ActionEvent<String,EN_StatusText>::Send ( s );
//}
//---------------------------------------------------------------------------
// Base class for all events. This class can be used for events that do not
// need to pass data. If you need to pass data, then you use the template
// class MinosEvent to wrap your data.
//---------------------------------------------------------------------------
class MinosEventBase
{
		MinosEventNumber FEventNo;
	public:
		MinosEventBase() : FEventNo ( EN_Unknown ) { }
		MinosEventBase ( MinosEventNumber e ) : FEventNo ( e ) { }

		virtual ~MinosEventBase() {}

		__property MinosEventNumber EventNo  = { read=FEventNo };

      bool operator== ( const MinosEventBase & rhs ) const
      {
			return FEventNo == rhs.FEventNo;
      }
      bool operator!= ( const MinosEventBase & rhs ) const
		{
         return !operator== ( rhs );
      }
	private:
};

//---------------------------------------------------------------------------
// This is the definition of the callback method used to process events.
//---------------------------------------------------------------------------
typedef void  ( __closure *TMinosEventCallback ) ( MinosEventBase & );
//---------------------------------------------------------------------------
// This is the template class you can use for passing event data. The class
// that you are wrapping up must have a default constructor and must have a
// copy constructor.
//---------------------------------------------------------------------------
template <class Tdata>
class MinosEvent : public MinosEventBase
{
      Tdata Data;
   public:
      __property Tdata Value  = { read=GetValue, write=SetValue };
      MinosEvent ( MinosEventNumber e, Tdata data ) : MinosEventBase ( e ), Data ( data ) { }
	private:
		void  SetValue ( Tdata value )
      {
         if ( Data != value )
			{
            Data = value;
         }
		}
      Tdata  GetValue()
      {
			return Data;
      }
};
//---------------------------------------------------------------------------
// This class is used by the MinosEventListener and MinosEventManager to hold
// details of the methods to call when events occur.
// You should use the MinosEventListener class to listen to events.
//---------------------------------------------------------------------------
class MinosEventListenerData
{
   public:

      MinosEventListenerData ( MinosEventNumber e,TMinosEventCallback m ) : Method ( m ), FEventNo ( e ) { }
      MinosEventListenerData() : Method ( 0 ), FEventNo ( EN_Unknown ) { }
      ~MinosEventListenerData() { }

      __property MinosEventNumber EventNo  = { read=FEventNo };

		bool operator== ( const MinosEventListenerData & rhs ) const
      {
			return FEventNo == rhs.FEventNo && Method == rhs.Method;
      }

		void Notify ( MinosEventBase &  e )
      {
         Method ( e );
		}

   private:
      MinosEventNumber FEventNo;
      TMinosEventCallback Method;

};
//---------------------------------------------------------------------------
// This class is used to manage events. It does not process the events itself,
// they are passed to other methods for processing. You use the StartLitening
// method to request that events are passed to a method for you to process.
// You should use the MinosEventListener class to listen to events.
//---------------------------------------------------------------------------
class MinosEventManager
{
		void  Remove(const MinosEventListenerData & d)
      {
			Listeners.remove(d);
      }

		std::list<MinosEventListenerData> Listeners;
       MinosEventManager() { }

	public:


		void  StartListening(const MinosEventListenerData & d)
		{
         Remove ( d );
         if ( d.EventNo != EN_Unknown )
            Listeners.push_back ( d );
      }
		void  StopListening(const MinosEventListenerData & d)
      {
			Remove (d);
      }

		void ProcessEvent (MinosEventBase & e)
      {
         std::list<MinosEventListenerData>::iterator i;
         i = Listeners.begin();
			while (i != Listeners.end())
         {
				if ((*i).EventNo == e.EventNo)
					(*i).Notify(e);
            i++;
			}
      }

		// MinosEventManager is a singleton
      static MinosEventManager & GetMinosEventManager ( void );
};
//---------------------------------------------------------------------------
// This class is used to manage a listener. It's important to stop listenening
// before the method that is called goes out of scope, hence the  need for
// MinosEventListener. This wraps up the calls to Start and Stop Litening. You
// include one MinosEventListener object in your form for each event you want to
// listen to.
//---------------------------------------------------------------------------
class MinosEventListener
{
   private:

      MinosEventListenerData Data;

   public:

      MinosEventListener ( MinosEventNumber e,TMinosEventCallback m ) : Data ( e,m )
      {
			MinosEventManager::GetMinosEventManager().StartListening ( Data );
      }

		MinosEventListener() { }

      void StartListening ( MinosEventNumber e,TMinosEventCallback m )
		{
         MinosEventManager & EM = MinosEventManager::GetMinosEventManager();
         if ( Data.EventNo != EN_Unknown )
				EM.StopListening ( Data );

         Data = MinosEventListenerData ( e,m );
         EM.StartListening ( Data );
      }

      ~MinosEventListener()
      {
         StopListening();
      }
		void StopListening ( void )
      {
         if ( Data.EventNo != EN_Unknown )
            MinosEventManager::GetMinosEventManager().StopListening ( Data );
      }
};
//---------------------------------------------------------------------------
template <class T,MinosEventNumber type>
class ActionEvent : public MinosEventBase
{
		T FData;

	public:

		ActionEvent ( T data )
            :  MinosEventBase ( type ),
            FData ( data )
            { }

		T getData() { return FData; }

		static void Send( T data );
};
template <class T,MinosEventNumber type>
void ActionEvent<T, type>::Send ( T data )
{
	ActionEvent<T,type> E ( data );
	MinosEventManager & EM = MinosEventManager::GetMinosEventManager();
	EM.ProcessEvent ( E );
}
//---------------------------------------------------------------------------
#endif
