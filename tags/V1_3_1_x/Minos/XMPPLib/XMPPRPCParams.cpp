/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#include "XMPP_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------
// RPC parameter structures
//---------------------------------------------------------------------------
RPCParam::RPCParam()
{}
RPCParam::~RPCParam()
{}
void RPCParam::addParam( TiXmlElement &node )
{
   TiXmlElement paramNode( "param" );
   addValue( paramNode );
   node.InsertEndChild( paramNode );
}
void RPCParam::addValue( TiXmlElement &node )
{
   TiXmlElement valueNode( "value" );
   addNode( valueNode );
   node.InsertEndChild( valueNode );
}
/*static*/ boost::shared_ptr<RPCParam> RPCParam::paramFactory(   TiXmlElement &node )
{
   // node points to the typed node we need to analyse

   if ( checkElementName( &node , "struct" ) )
   {
      RPCParam *p = new RPCParamStruct( node );
      return boost::shared_ptr<RPCParam>(p);
   }
   else
      if ( checkElementName( &node , "array" ) )
      {
         RPCParam *p = new RPCParamArray(node);
         return boost::shared_ptr<RPCParam>(p);
      }

      else
         if ( checkElementName( &node , "int" ) || checkElementName( &node , "i4" ) )
         {
            return boost::shared_ptr<RPCParam>(new RPCIntParam(node));
         }
         else
            if ( checkElementName( &node , "boolean" ) )
            {
               return boost::shared_ptr<RPCParam>(new RPCBooleanParam(node));
            }
            else
               if ( checkElementName( &node , "double" ) )
               {
                  return boost::shared_ptr<RPCParam>(new RPCDoubleParam(node));
               }
               else
                  if ( checkElementName( &node , "string" ) )
                  {
                     return boost::shared_ptr<RPCParam>(new RPCStringParam(node));
                  }
                  else
                     if ( checkElementName( &node , "base64" ) )
                     {
                        return boost::shared_ptr<RPCParam>(new RPCBase64Param(node));
                     }
                     else
                        if ( checkElementName( &node , "dateTime.iso8601" ) )
                        {
                           return boost::shared_ptr<RPCParam>(new RPCDtgParam(node));
                        }
   return boost::shared_ptr<RPCParam>((RPCParam *)0);
}
bool RPCParam::getBoolean( bool & )
{
   return false;
}
bool RPCParam::getInt( int & )
{
   return false;
}
bool RPCParam::getDouble( double & )
{
   return false;
}
bool RPCParam::getString( std::string & )
{
   return false;
}
bool RPCParam::getBase64( std::string & )
{
   return false;
}
bool RPCParam::getDtg( std::string & )
{
   return false;
}
bool RPCParam::getMember( const std::string &, boost::shared_ptr<RPCParam>& )
{
   return false;
}
bool RPCParam::getMember( unsigned int , boost::shared_ptr<RPCParam>& )
{
   return false;
}
bool RPCParam::getElements( unsigned int & )
{
   return false;
}
bool RPCParam::getElement( unsigned int , boost::shared_ptr<RPCParam>& )
{
   return false;
}
void RPCParam::addMember( boost::shared_ptr<RPCParam> , const std::string &name )
{
   throw Exception("addMember to non-struct RPCParam");
}

void RPCParam::addMember( int, const std::string &name )
{
   throw Exception("addMember to non-struct RPCParam");
}
void RPCParam::addMember( bool, const std::string &name )
{
   throw Exception("addMember to non-struct RPCParam");
}
void RPCParam::addMember( double, const std::string &name )
{
   throw Exception("addMember to non-struct RPCParam");
}
void RPCParam::addMember( const std::string &, const std::string &name )
{
   throw Exception("addMember to non-struct RPCParam");
}
void RPCParam::addBase64Member( const std::string &, const std::string &name )
{
   throw Exception("addMember to non-struct RPCParam");
}
void RPCParam::addDtgMember( const std::string &, const std::string &name )
{
   throw Exception("addMember to non-struct RPCParam");
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// structure
RPCParamStruct::RPCParamStruct()
{}
RPCParamStruct::RPCParamStruct( TiXmlElement &aNode )
{
   // struct is list of members, each with name and value
   for ( TiXmlElement * e = aNode.FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      if ( checkElementName( e , "member" ) )
      {
         std::string pname;
         TiXmlElement *valuenode = 0;
         for ( TiXmlElement * f = e->FirstChildElement(); f; f = f->NextSiblingElement() )
         {
            if ( checkElementName( f, "value" ) )
            {
               valuenode = f;
            }
            if ( checkElementName( f, "name" ) )
            {
               pname = f->GetText( );
            }
         }
         if ( valuenode && pname.size() )
         {
            TiXmlElement * vnode = valuenode->FirstChildElement();
            if ( vnode )
            {
               addMember( paramFactory( *vnode ), pname );
            }
         }
      }
   }
}
RPCParamStruct::~RPCParamStruct()
{
   elements.clear(); // clears the shared pointers
}

void RPCParamStruct::addMember( boost::shared_ptr<RPCParam>p, const std::string &name )
{
   p->name = name;
   elements.push_back( p );
}
void RPCParamStruct::addMember( int v, const std::string &name )
{
   addMember( boost::shared_ptr<RPCParam>(new RPCIntParam( v )), name );
}
void RPCParamStruct::addMember( bool v, const std::string &name )
{
   addMember( boost::shared_ptr<RPCParam>(new RPCBooleanParam( v )), name );
}
void RPCParamStruct::addMember( double v, const std::string &name )
{
   addMember( boost::shared_ptr<RPCParam>(new RPCDoubleParam( v )), name );
}
void RPCParamStruct::addMember( const std::string &v, const std::string &name )
{
   addMember( boost::shared_ptr<RPCParam>(new RPCStringParam( v )), name );
}
void RPCParamStruct::addBase64Member( const std::string &v, const std::string &name )
{
   addMember( boost::shared_ptr<RPCParam>(new RPCBase64Param( v )), name );
}
void RPCParamStruct::addDtgMember( const std::string &v, const std::string &name )
{
   addMember( boost::shared_ptr<RPCParam>(new RPCDtgParam( v )), name );
}

void RPCParamStruct::addNode( TiXmlElement &node )
{
   // struct
   // member
   // name

   // value as for individual
   TiXmlElement sNode( "struct" );
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = elements.begin(); i != elements.end(); i++ )
   {
      TiXmlElement mNode( "member" );
      TiXmlElement nNode( "name" );
      TiXmlText tNode( ( *i ) ->name.c_str() );
      nNode.InsertEndChild( tNode );
      mNode.InsertEndChild( nNode );
      ( *i ) ->addValue( mNode );
      sNode.InsertEndChild( mNode );
   }
   node.InsertEndChild( sNode );

}
std::string RPCParamStruct::print()
{
   std::string s = "Struct\r\n";
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = elements.begin(); i != elements.end(); i++ )
   {
      s += "Member name " + ( *i ) ->name + "\r\n";
      s += ( *i ) ->print();
   }
   s += "End Struct\r\n";
   return s;
}
std::string RPCParamStruct::analyse()
{
   std::string s;
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = elements.begin(); i != elements.end(); i++ )
   {
      s += ( *i ) ->name + "<" + ( *i ) ->analyse() + ">";
   }
   return s;
}

bool RPCParamStruct::getMember( const std::string &name, boost::shared_ptr<RPCParam>&p )
{
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = elements.begin(); i != elements.end(); i++ )
   {
      if ( ( *i ) ->name == name )
      {
         p = ( *i );
         return true;
      }
   }
   return false;
}
bool RPCParamStruct::getMember( unsigned int eleno, boost::shared_ptr<RPCParam>&p )
{
   if ( eleno < elements.size() )
   {
      p = elements[ eleno ];
      return true;
   }
   return false;
}
bool RPCParamStruct::getElements( unsigned int &size )
{
   size = ( unsigned int ) elements.size();
   return true;
}
bool RPCParamStruct::getElement( unsigned int eleno, boost::shared_ptr<RPCParam>&p )
{
   if ( eleno < elements.size() )
   {
      p = elements[ eleno ];
      return true;
   }
   return false;
}

//---------------------------------------------------------------------------
// array - not being used at the moment...
RPCParamArray::RPCParamArray()
{}
RPCParamArray::RPCParamArray( TiXmlElement &aNode )
{
   // array is list of values
   // Array
   //    data
   //       value
   //       value
   //       value
   for ( TiXmlElement * e = aNode.FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      if ( checkElementName( e , "data" ) )
      {
         std::string pname;
         for ( TiXmlElement * f = e->FirstChildElement(); f; f = f->NextSiblingElement() )
         {
            if ( checkElementName( f, "value" ) )
            {
               TiXmlElement * vnode = f->FirstChildElement();
               if ( vnode )
               {
                  addElement( paramFactory( *vnode ) );
               }
            }
         }
         break;
      }
   }

}
RPCParamArray::~RPCParamArray()
{
   elements.clear(); // clears shared pointers
}

void RPCParamArray::addElement( boost::shared_ptr<RPCParam> p )
{
   elements.push_back( p );
}
void RPCParamArray::addElement( int v )
{
   addElement( boost::shared_ptr<RPCParam>(new RPCIntParam( v )) );
}
void RPCParamArray::addElement( bool v )
{
   addElement( boost::shared_ptr<RPCParam>(new RPCBooleanParam( v )) );
}
void RPCParamArray::addElement( double v )
{
   addElement( boost::shared_ptr<RPCParam>(new RPCDoubleParam( v )) );
}
void RPCParamArray::addElement( const char *v )
{
   addElement( boost::shared_ptr<RPCParam>(new RPCStringParam( v )) );
}
void RPCParamArray::AddBase64Element( const char *v )
{
   addElement( boost::shared_ptr<RPCParam>(new RPCBase64Param( v )) );
}
void RPCParamArray::AddDtgElement( const char *v )
{
   addElement( boost::shared_ptr<RPCParam>(new RPCDtgParam( v )) );
}

void RPCParamArray::addNode( TiXmlElement &node )
{
   TiXmlElement aNode( "array" );
   TiXmlElement dNode( "data" );
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = elements.begin(); i != elements.end(); i++ )
   {
      ( *i ) ->addValue( dNode );
   }
   aNode.InsertEndChild( dNode );
   node.InsertEndChild( aNode );
}
std::string RPCParamArray::print()
{
   std::string s = "Array\r\n";
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = elements.begin(); i != elements.end(); i++ )
   {
      s += ( *i ) ->print();
   }
   s += "End Array\r\n";
   return s;
}
std::string RPCParamArray::analyse()
{
   std::string s = "[";
   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = elements.begin(); i != elements.end(); i++ )
   {
      s += ( *i ) ->analyse();
   }
   s += "]";
   return s;
}
bool RPCParamArray::getElements( unsigned int &size )
{
   size = ( unsigned int ) elements.size();
   return true;
}
bool RPCParamArray::getElement( unsigned int eleno, boost::shared_ptr<RPCParam> &p )
{
   if ( elements.size() > eleno )
   {
      p = elements[ eleno ];
      return true;
   }
   return false;
}

//---------------------------------------------------------------------------
// int or i4
RPCIntParam::RPCIntParam( int v ) : value( v )
{}
RPCIntParam::RPCIntParam() : value( 0 )
{}
RPCIntParam::RPCIntParam( TiXmlElement &aNode )
{
   const char * val = aNode.GetText();
   if ( val )
   {
      value = atoi( val );
   }
}
RPCIntParam::~RPCIntParam()
{}
void RPCIntParam::addNode( TiXmlElement &node )
{
   char buff[ 40 ];
   sprintf( buff, "%d", value );
   TiXmlElement vNode( "i4" );
   TiXmlText tNode( buff );
   vNode.InsertEndChild( tNode );
   node.InsertEndChild( vNode );
}
std::string RPCIntParam::print()
{
   char buff[ 128 ];
   sprintf( buff, "Integer value %d\r\n", value );
   return std::string( buff );
}
std::string RPCIntParam::analyse()
{
   char buff[ 128 ];
   sprintf( buff, "%d", value );
   return std::string( buff );
}
bool RPCIntParam::getInt( int &res )
{
   res = value;
   return true;
}

//---------------------------------------------------------------------------
// boolean
RPCBooleanParam::RPCBooleanParam( bool v ) : value( v )
{}
RPCBooleanParam::RPCBooleanParam() : value( false )
{}
RPCBooleanParam::RPCBooleanParam( TiXmlElement &aNode )
{
   const char * data = aNode.GetText();
   if ( data )
   {
      value = ( strlen( data ) && data[ 0 ] == '0' ) ? false : true;
   }
}
RPCBooleanParam::~RPCBooleanParam()
{}
void RPCBooleanParam::addNode( TiXmlElement &node )
{
   TiXmlElement vNode( "boolean" );
   TiXmlText tNode( value ? "1" : "0" );
   vNode.InsertEndChild( tNode );
   node.InsertEndChild( vNode );

}
std::string RPCBooleanParam::print()
{
   if ( value )
      return "Boolean value true\r\n";
   return "Boolean value false\r\n";
}
std::string RPCBooleanParam::analyse()
{
   if ( value )
      return "true";
   return "false";
}
bool RPCBooleanParam::getBoolean( bool &res )
{
   res = value;
   return true;
}

//---------------------------------------------------------------------------
// double
RPCDoubleParam::RPCDoubleParam( double v ) : value( v )
{}
RPCDoubleParam::RPCDoubleParam() : value( 0 )
{}
RPCDoubleParam::RPCDoubleParam( TiXmlElement &aNode )
{
   const char * data = aNode.GetText();
   if ( data )
   {
      char * endptr;
      value = strtod( data, &endptr );    // we may want to detect errors...
   }
}
RPCDoubleParam::~RPCDoubleParam()
{}
void RPCDoubleParam::addNode( TiXmlElement &node )
{
   TiXmlElement vNode( "double" );
   char buff[ 40 ];
   sprintf( buff, "%f", value );
   TiXmlText tNode( buff );
   vNode.InsertEndChild( tNode );
   node.InsertEndChild( vNode );
}
std::string RPCDoubleParam::print()
{
   char buff[ 128 ];
   sprintf( buff, "double value %f\r\n", value );
   return std::string( buff );
}
std::string RPCDoubleParam::analyse()
{
   char buff[ 128 ];
   sprintf( buff, "%f", value );
   return std::string( buff );
}
bool RPCDoubleParam::getDouble( double &res )
{
   res = value;
   return true;
}

//---------------------------------------------------------------------------
// string
RPCStringParam::RPCStringParam( std::string v ) : value( v )
{}
RPCStringParam::RPCStringParam()
{}
RPCStringParam::RPCStringParam( TiXmlElement &aNode )
{
   const char * val = aNode.GetText();
   if ( val )
   {
      value = val;
   }
}
RPCStringParam::~RPCStringParam()
{}
void RPCStringParam::addNode( TiXmlElement &node )
{
   TiXmlElement vNode( "string" );

   /*
         std::string Out;
         int nIn = 0;
      
         int nInLen = value.size();
         while ( nIn < nInLen )
         {
            switch ( value[ nIn ] )
            {
               default:
                  {
                     Out += value[ nIn++ ];
                     break;
                  }
               case '&':
                  {
                     Out += "&amp;";
                     nIn++;
                     break;
                  }
      
               case '>':
                  {
                     Out += "&gt;";
                     nIn++;
                     break;
                  }
      
               case '<':
                  {
                     Out += "&lt;";
                     nIn++;
                     break;
                  }
      
               case '\"':
                  {
                     Out += "&quot;";
                     nIn++;
                     break;
                  }
      
            }
         }
         // We need to add XML character escaping here
         TiXmlText tNode( Out );
   */
   TiXmlText tNode( value );
   vNode.InsertEndChild( tNode );
   node.InsertEndChild( vNode );
}
std::string RPCStringParam::print()
{
   std::string p = "String value \"" + value + "\"\r\n";
   return p;
}
std::string RPCStringParam::analyse()
{
   return "'" + value + "'";
}
bool RPCStringParam::getString( std::string &res )
{
   res = value;
   return true;
}

//---------------------------------------------------------------------------
// <dateTime.iso8601>
RPCDtgParam::RPCDtgParam( std::string v ) : value( v )
{}
RPCDtgParam::RPCDtgParam()
{}
RPCDtgParam::RPCDtgParam( TiXmlElement &aNode )
{
   const char * val = aNode.GetText();
   if ( val )
   {
      value = val;
   }
}
RPCDtgParam::~RPCDtgParam()
{}
void RPCDtgParam::addNode( TiXmlElement &node )
{
   TiXmlElement vNode( "dateTime.iso8601" );
   TiXmlText tNode( value );
   vNode.InsertEndChild( tNode );
   node.InsertEndChild( vNode );
}
std::string RPCDtgParam::print()
{
   return "dateTime.iso8601 value \"" + value + "\"\r\n";
}
std::string RPCDtgParam::analyse()
{
   return value;
}
bool RPCDtgParam::getDtg( std::string &res )
{
   res = value;
   return true;
}
//---------------------------------------------------------------------------
// base64
// This code assumes that we are base64'ing a string, rather than binary...
// NOT a correct assumption!

RPCBase64Param::RPCBase64Param( std::string v ) : value( v )
{
   //   char * b64 = iks_base64_encode( v.c_str(), ( unsigned int ) ( v.length() ) );
   //   value = b64;
   //   iks_free( b64 );
}
RPCBase64Param::RPCBase64Param()
{}
RPCBase64Param::RPCBase64Param( TiXmlElement &aNode )
{

   const char * val = aNode.GetText();
   if ( val )
   {
      value = val;
      //   value = base64_decode( b64 );
   }

}
RPCBase64Param::~RPCBase64Param()
{}
void RPCBase64Param::addNode( TiXmlElement &node )
{
   TiXmlElement vNode( "base64" );
   TiXmlText tNode( value );
   vNode.InsertEndChild( tNode );
   node.InsertEndChild( vNode );
}
std::string RPCBase64Param::print()
{
   return "Base64 value \"" + value + "\"\r\n";
}
std::string RPCBase64Param::analyse()
{
   return value;
}
bool RPCBase64Param::getBase64( std::string &res )
{
   res = value;
   return true;
}
//---------------------------------------------------------------------------

RPCArgs::RPCArgs()
{}
RPCArgs::~RPCArgs()
{
   args.clear();// clears shared pointers
}
void RPCArgs::addParam( RPCParam *  )
{
   throw Exception("Add param by pointer to RPCArgs");
}
void RPCArgs::addParam( boost::shared_ptr<RPCParam> p )
{
   args.push_back( p );
}
void RPCArgs::addParam( int v )
{
   addParam( boost::shared_ptr<RPCParam>(new RPCIntParam(v)));
}
void RPCArgs::addParam( bool v )
{
   addParam( boost::shared_ptr<RPCParam>(new RPCBooleanParam( v )) );
}
void RPCArgs::addParam( double v )
{
   addParam( boost::shared_ptr<RPCParam>(new RPCDoubleParam( v )) );
}
void RPCArgs::addParam( const std::string &v )
{
   addParam( boost::shared_ptr<RPCParam>(new RPCStringParam( v )) );
}
void RPCArgs::addBase64Param( const std::string &v )
{
   addParam( boost::shared_ptr<RPCParam>(new RPCBase64Param( v )) );
}
void RPCArgs::addDtgParam( const std::string &v )
{
   addParam( boost::shared_ptr<RPCParam>(new RPCDtgParam( v )) );
}

TiXmlElement *RPCArgs::makeParamsNode( )
{
   TiXmlElement * pNode = new TiXmlElement( "params" );

   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = args.begin(); i != args.end(); i++ )
   {
      ( *i ) ->addParam( *pNode );
   }

   return pNode;

}
std::string RPCArgs::makeParamsString()
{
   TiXmlElement * params = makeParamsNode( );
   std::string s;
   s << *params;
   delete params;
   return s;
}

void RPCArgs::addParams( TiXmlElement &paramsNode )
{
   if ( UTF8XML.length() )
   {
      TiXmlBase::SetCondenseWhiteSpace( false );
      TiXmlDocument paramdoc;
      paramdoc.Parse( UTF8XML.c_str(), 0 );
      TiXmlElement *root = paramdoc.RootElement();
      paramsNode.InsertEndChild( *root );

   }
   else
   {
      TiXmlElement *pNode = makeParamsNode( );
      paramsNode.InsertEndChild( *pNode );     // which leaks pnode, as it gets cloned
      delete pNode;
   }
}
bool RPCArgs::parseParams( std::string UTF8XML )   // parse from the string to args
{
   // And now we move to tinyXML for parsing...
   TiXmlBase::SetCondenseWhiteSpace( false );
   TiXmlDocument paramdoc;
   paramdoc.Parse( UTF8XML.c_str(), 0 );
   TiXmlElement *root = paramdoc.RootElement();
   if ( !root )
   {
      //      err = "No XML root element (does file exist?)";
      return false;
   }
   if ( !checkElementName( root, "params" ) )
   {
      //      err = "Invalid XML root element";
      return false;
   }
   for ( TiXmlElement * e = root->FirstChildElement(); e; e = e->NextSiblingElement() )
   {
      // declaration, destination lines
      if ( checkElementName( e, "param" ) )
      {
         // value node is the child of <param />
         TiXmlElement * vnode = e->FirstChildElement();
         if ( vnode )
         {
            // type node is the child of <value />
            TiXmlElement * pnode = vnode->FirstChildElement();
            if ( pnode )
            {
               // and then pass the type node on to a param factory
               boost::shared_ptr<RPCParam> p = RPCParam::paramFactory( *pnode );
               // add each one to args
               if ( p )
                  args.push_back( p );
            }
         }
      }
   }
   return true;
}
std::string RPCArgs::PrintArgs()
{
   std::string s;

   for ( std::vector<boost::shared_ptr<RPCParam> >::iterator i = args.begin(); i != args.end(); i++ )
   {
      s += ( *i ) ->print();
   }
   return s;
}
bool RPCArgs::getStructArgMember( unsigned int argno, const std::string &name, boost::shared_ptr<RPCParam>&res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getMember( name, res );
   }
   return false;
}
bool RPCArgs::getStructArgMember( unsigned int argno, unsigned int eleno, boost::shared_ptr<RPCParam>&res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getMember( eleno, res );
   }
   return false;
}
bool RPCArgs::getArrayArgElements( unsigned int argno, unsigned int &size )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getElements( size );
   }
   return false;
}
bool RPCArgs::getArrayArgElement( unsigned int argno, unsigned int eleno, boost::shared_ptr<RPCParam>&res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getElement( eleno, res );
   }
   return false;
}
bool RPCArgs::getBooleanArg( unsigned int argno, bool &res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getBoolean( res );
   }
   return false;
}
bool RPCArgs::getIntArg( unsigned int argno, int &res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getInt( res );
   }
   return false;
}
bool RPCArgs::getDoubleArg( unsigned int argno, double &res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getDouble( res );
   }
   return false;
}
bool RPCArgs::getStringArg( unsigned int argno, std::string &res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getString( res );
   }
   return false;
}
bool RPCArgs::getBase64Arg( unsigned int argno, std::string &res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getBase64( res );
   }
   return false;
}
bool RPCArgs::getDtgArg( unsigned int argno, std::string &res )
{
   if ( args.size() > argno )
   {
      return args[ argno ] ->getDtg( res );
   }
   return false;
}

