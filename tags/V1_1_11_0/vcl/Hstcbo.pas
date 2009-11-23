{ -------------------------------------------------------------------------------------}
{ An "History ComboBox" component for Delphi32.                                        }
{ Copyright 1996, Christian Holzner.  All Rights Reserved.                             }
{ This component can be freely used and distributed in commercial and private          }
{ environments, provided this notice is not modified in any way.                       }
{ -------------------------------------------------------------------------------------}
{ Feel free to contact us if you have any questions, comments or suggestions at        }
{ cholzner@ping.at (Christian Holzner)                                                 }
{ JFConnault@mail.dotcom.fr (Jean-Fabien Connault)                                     }
{ -------------------------------------------------------------------------------------}
{ Date last modified:  10/20/96                                                        }
{ -------------------------------------------------------------------------------------}

{ -------------------------------------------------------------------------------------}
{ THistoryComboBox v1.61                                                               }
{ -------------------------------------------------------------------------------------}
{ Description:                                                                         }
{   A component that allows you to Read/Write Items of a TComboBox to/from Registry    }
{ Added Properties to ComboBox:                                                        }
{   property Key: String;                    "Key in Registry"                         }
{   property MaxHistoryLength: Integer;      "Max allowed length of the history-items  }
{   property RootKey: TRootKey;                                                        }
{ Added Procedures to ComboBox:                                                        }
{   procedure ReadRegistry;                  "Reads items from Registry"               }
{   procedure WriteRegistry;                 "Writes items to Registry"                }
{   procedure AddToList;                     "Adding an item to the Historylistbox"    }
{                                                                                      }
{ See example contained in example.zip file for more details.                          }
{ -------------------------------------------------------------------------------------}
{ Revision History:                                                                    }
{ 1.00:  + Uses Ini-file for storage by Christian Holzner                              }
{ 1.50:  + Uses Registry for storage by Christian Holzner                              }
{ 1.60:  + Added RootKey property by Jean-Fabien Connault                              }
{ 1.61:  + Fixed some bugs by Jean-Fabien Connault                                     }
{ -------------------------------------------------------------------------------------}

unit Hstcbo;
{$ObjExportAll On}

interface

uses
  SysUtils, Windows, Classes, StdCtrls, Forms, Registry, Dialogs;

type
 TRootKey = (hkClassesRoot, hkCurrentConfig, hkCurrentUser, hkDynData,
             hkLocalMachine,hkUsers);

 THistoryComboBox = class(TComboBox)
  private
    FKey: String;
    FMaxHistoryLength: Integer;
    FRootKey: TRootKey;
    procedure SetRootKey(ARootKey: TRootKey);
protected
    { Protected-Deklarationen }
  public
    constructor Create(AOwner: TComponent); Override;
    procedure ReadRegistry;
    procedure WriteRegistry;
    procedure AddToList(NewItem: String);
  published
    property Key: String
             read FKey write FKey;
    property MaxHistoryLength: Integer
             read FMaxHistoryLength write FMaxHistoryLength default 9;
    property RootKey: TRootKey read FRootKey write SetRootKey default hkCurrentUser;

  end;


procedure Register;

const
RootKeyValues: array[TRootKey] of integer =
(HKey_Classes_Root, HKey_Current_Config, HKey_Current_User, HKey_Dyn_Data,
              HKey_Local_Machine, HKey_Users);

implementation


constructor THistoryComboBox.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FMaxHistoryLength := 9;
  FRootKey := hkCurrentUser;
end;

procedure THistoryComboBox.SetRootKey(ARootKey: TRootKey);
begin
  if FRootKey <> ARootKey then
     FRootKey := ARootKey;
end;

procedure THistoryComboBox.ReadRegistry;
{ Reads items from Registry }
var
  Registry: TRegistry;
  i : Integer;
  Value: String;
begin
  if FKey <> '' then begin

    Registry := TRegistry.Create;
    try
      Clear;
      { Read maximum allowed entries from Registry }
      Registry.RootKey := RootKeyValues[FRootKey];
      if Registry.OpenKey('\'+FKey,False) then begin
        for i:= 0 to FMaxHistoryLength-1 do begin
           Value := Registry.ReadString('H'+IntToStr(i));
           If Value <> ''
             then Items.Add(Value)
             else break;
        end;
      end;
    finally
      Registry.Free;
    end;

  end;
end;


procedure THistoryComboBox.WriteRegistry;
{ Writes items to registry }
var
  Registry: TRegistry;
  i : Integer;
begin
  if FKey <> '' then begin

    Registry := TRegistry.Create;
    try
      { Read maximum allowed entries from Registry }
      Registry.RootKey :=  RootKeyValues[FRootKey];
      if Registry.OpenKey('\'+FKey,False) then begin
        try
          for i:= 0 to FMaxHistoryLength-1 do
             Registry.ReadString('H'+IntToStr(i));
        except
          on ERegistryException do;
        end;
      end;

      { Erase existing entries to get rid of more then the max. allowed entries }
      if Registry.OpenKey('\',False) then
         Registry.DeleteKey(FKey);

      { Add to Historylistbox }
      AddToList(Text);
      { Write to Registry }
      if Registry.OpenKey('\'+FKey,True) then
      for i := 0 to Items.Count-1 do
         Registry.WriteString('H'+IntToStr(i),Items[i]);
    finally
      Registry.Free;
    end;

  end;
end;


procedure THistoryComboBox.AddToList(NewItem: String);
{ Add a string to the Historylistbox }
var
   i, nb_elem: integer;
begin
   if NewItem <> '' then
    begin
     { Insert in first position }
     Items.Insert(0,NewItem);
     { Check maximum numer of entries and delete any duplicate }
     i := 1;
     nb_elem := Items.Count;
     while i < nb_elem do
      begin
       if (Items[i] = NewItem) or (i > FMaxHistoryLength-1) then
        begin
         Items.Delete(i);
         dec(nb_elem);
        end
       else
        inc(i);
      end;
     Text := items[0];
   end;
end;


procedure Register;
begin
  RegisterComponents('MJGAddons',[THistoryComboBox]);
end;

end.



