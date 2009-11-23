/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef XMPPRPCParamsH
#define XMPPRPCParamsH

//---------------------------------------------------------------------------
// RPC parameter structures

class TiXmlElement;
class RPCParam
{
   public:
      std::string name;

      RPCParam();
      virtual ~RPCParam();
      virtual void addParam( TiXmlElement &node );
      virtual void addValue( TiXmlElement &node );
      virtual void addNode( TiXmlElement &node ) = 0;
      virtual std::string print() = 0;
      virtual std::string analyse() = 0;

      static boost::shared_ptr<RPCParam> paramFactory( TiXmlElement &node );

      virtual bool getMember( const std::string &name, boost::shared_ptr<RPCParam> &p );
      virtual bool getMember( unsigned int eleno, boost::shared_ptr<RPCParam> &p );
      virtual bool getElements( unsigned int &size );
      virtual bool getElement( unsigned int eleno, boost::shared_ptr<RPCParam> &p );
      virtual bool getBoolean( bool &res );
      virtual bool getInt( int &res );
      virtual bool getDouble( double &res );
      virtual bool getString( std::string &res );
      virtual bool getBase64( std::string &res );
      virtual bool getDtg( std::string &res );

      virtual void addMember( boost::shared_ptr<RPCParam> , const std::string &name );
      virtual void addMember( int, const std::string &name );
      virtual void addMember( bool, const std::string &name );
      virtual void addMember( double, const std::string &name );
      virtual void addMember( const std::string &, const std::string &name );
      virtual void addBase64Member( const std::string &, const std::string &name );
      virtual void addDtgMember( const std::string &, const std::string &name );
};

// structure
class RPCParamStruct: public RPCParam
{
      void addMember( RPCParam * , const std::string &name );
   protected:
      std::vector <boost::shared_ptr<RPCParam> > elements;
   public:
      RPCParamStruct();
      RPCParamStruct( TiXmlElement &sNode );
      virtual ~RPCParamStruct();

      void addMember( boost::shared_ptr<RPCParam> , const std::string &name );
      void addMember( int, const std::string &name );
      void addMember( bool, const std::string &name );
      void addMember( double, const std::string &name );
      void addMember( const std::string &, const std::string &name );
      void addBase64Member( const std::string &, const std::string &name );
      void addDtgMember( const std::string &, const std::string &name );

      virtual void addNode( TiXmlElement &node );
      virtual std::string print();
      virtual std::string analyse();

      virtual bool getElements( unsigned int &size );
      virtual bool getElement( unsigned int eleno, boost::shared_ptr<RPCParam> &p );
      virtual bool getMember( const std::string &name, boost::shared_ptr<RPCParam> &p );
      virtual bool getMember( unsigned int eleno, boost::shared_ptr<RPCParam> &p );
};

// array
class RPCParamArray: public RPCParam
{
      void addElement( RPCParam * );
   protected:
      std::vector <boost::shared_ptr<RPCParam> > elements;
   public:
      RPCParamArray();
      RPCParamArray( TiXmlElement &sNode );
      virtual ~RPCParamArray();

      void addElement( boost::shared_ptr<RPCParam>  );

      void addElement( int );
      void addElement( bool );
      void addElement( double );
      void addElement( const char * );
      void AddBase64Element( const char * );
      void AddDtgElement( const char * );

      virtual void addNode( TiXmlElement &node );
      static boost::shared_ptr<RPCParamArray> ParseArray( TiXmlElement &aNode );
      virtual std::string print();
      virtual std::string analyse();
      virtual bool getElements( unsigned int &size );
      virtual bool getElement( unsigned int eleno, boost::shared_ptr<RPCParam> &p );
};

// int or i4
class RPCIntParam: public RPCParam
{
   protected:
      int value;
   public:
      RPCIntParam( int v );
      RPCIntParam( TiXmlElement &sNode );
      RPCIntParam();
      virtual ~RPCIntParam();
      virtual void addNode( TiXmlElement &node );
      virtual std::string print();
      virtual std::string analyse();

      virtual bool getInt( int &res );
};

// boolean
class RPCBooleanParam: public RPCParam
{
   protected:
      bool value;
   public:
      RPCBooleanParam( bool v );
      RPCBooleanParam( TiXmlElement &sNode );
      RPCBooleanParam();
      virtual ~RPCBooleanParam();
      virtual void addNode( TiXmlElement &node );
      virtual std::string print();
      virtual std::string analyse();

      virtual bool getBoolean( bool &res );
};

// double
class RPCDoubleParam: public RPCParam
{
   protected:
      double value;
   public:
      RPCDoubleParam( double v );
      RPCDoubleParam( TiXmlElement &sNode );
      RPCDoubleParam();
      virtual ~RPCDoubleParam();
      virtual void addNode( TiXmlElement &node );
      virtual std::string print();
      virtual std::string analyse();

      virtual bool getDouble( double &res );
};

// string
class RPCStringParam: public RPCParam
{
   protected:
      std::string value;
   public:
      RPCStringParam( std::string v );
      RPCStringParam( TiXmlElement &sNode );
      RPCStringParam();
      virtual ~RPCStringParam();
      virtual void addNode( TiXmlElement &node );
      virtual std::string print();
      virtual std::string analyse();

      virtual bool getString( std::string &res );
};

// <dateTime.iso8601>
class RPCDtgParam: public RPCParam
{
   protected:
      std::string value;   // not we want eventually, but...
   public:
      RPCDtgParam( std::string v );
      RPCDtgParam( TiXmlElement &sNode );
      RPCDtgParam();
      virtual ~RPCDtgParam();
      virtual void addNode( TiXmlElement &node );
      virtual std::string print();
      virtual std::string analyse();

      virtual bool getDtg( std::string &res );
};

// base64
class RPCBase64Param: public RPCParam
{
   protected:
      std::string value;
   public:
      RPCBase64Param( std::string v );
      RPCBase64Param( TiXmlElement &sNode );
      RPCBase64Param();
      virtual ~RPCBase64Param();
      virtual void addNode( TiXmlElement &node );
      virtual std::string print();
      virtual std::string analyse();

      virtual bool getBase64( std::string &res );
};

class RPCArgs
{
      void addParam( RPCParam *  );
   public:
      RPCArgs();
      virtual ~RPCArgs();

      virtual std::string PrintArgs();

      std::vector <boost::shared_ptr<RPCParam> > args;
      std::string UTF8XML; // pre-built params structure

      void addParam( boost::shared_ptr<RPCParam> );
      void addParam( int );
      void addParam( bool );
      void addParam( double );
      void addParam( const std::string & );
      void addBase64Param( const std::string & );
      void addDtgParam( const std::string & );

      TiXmlElement * makeParamsNode(  );
      std::string makeParamsString();
      void addParams( TiXmlElement &paramsNode );       // add args to the node

      bool parseParams( std::string UTF8XML ); // parse from the string to args

      bool getStructArgMember( unsigned int argno, const std::string &name, boost::shared_ptr<RPCParam>  &res );
      bool getStructArgMember( unsigned int argno, unsigned int eleno, boost::shared_ptr<RPCParam>  &res );
      bool getArrayArgElements( unsigned int argno, unsigned int &size );
      bool getArrayArgElement( unsigned int argno, unsigned int eleno, boost::shared_ptr<RPCParam> &res );

      bool getBooleanArg( unsigned int argno, bool &res );
      bool getIntArg( unsigned int argno, int &res );
      bool getDoubleArg( unsigned int argno, double &res );
      bool getStringArg( unsigned int argno, std::string &res );
      bool getBase64Arg( unsigned int argno, std::string &res );
      bool getDtgArg( unsigned int argno, std::string &res );
};

//---------------------------------------------------------------------------
#endif
