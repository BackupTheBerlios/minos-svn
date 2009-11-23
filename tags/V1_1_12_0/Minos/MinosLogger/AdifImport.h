/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		VHF Contest Adjudication
//
// COPYRIGHT         M. J. Goodey, 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
#ifndef AdifImportH
 #define AdifImportH

class ADIFImport
{
      int offset;
      int limit;
      bool getNextChar( char &ic );

      ADIFImport( LoggerContestLog * c, HANDLE hFile );
      ~ADIFImport();

      bool importAdifFile();
      bool executeImport();

      void ADIFImportFieldDecode( AnsiString Fieldname, int FieldLength, AnsiString FieldType, AnsiString FieldContent );
      void ADIFImportEndOfRecord( );

      HANDLE adifContestFile;
      String fileContent;

      LoggerContestLog * acontest;
      DisplayContestContact *aqso;
      int next_block;
   public:
      static bool doImportADIFLog( LoggerContestLog * c, HANDLE hFile );

};

#endif
