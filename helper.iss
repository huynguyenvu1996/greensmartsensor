  //Exec(ExpandConstant('{app}\{#PathUninstall}\{#AppExeProcessDetectorName}'),
  //'"' + ExpandConstant('{app}\{#PathUninstall}\{#ProcessDetectorFileName}') + '" "' 
  //+ ExpandConstant('{app}\{#PathLocalJREBin}\javaw.exe') + '" "' 
  //+ ExpandConstant('{app}\{#PathChromeNativeMessageHost}\{#ChromeNativeMessageFormExeName}') + '" "' 
  //+ ExpandConstant('{app}\{#PathLocalJREBin}\java.exe') 
  //+ '"','',SW_HIDE,ewWaitUntilTerminated, ResultCode);
  //Sleep(2000)
  //Result := DetectProcessIsRunning(ExpandConstant('{app}\{#PathUninstall}\{#ProcessDetectorFileName}'));

// https://stackoverflow.com/questions/43900649/how-to-read-multiple-xml-nodes-inno-setup
function DetectProcessIsRunning(FileName: string): Boolean;
var
  XMLNode: Variant;
  XMLNodeList: Variant;
  XMLDocument: Variant;
  ContentMessage : string;
  Index, ResultCode: Integer;  
begin
  Result := true;
  XMLDocument := CreateOleObject('Msxml2.DOMDocument.6.0');
  try
    XMLDocument.async := False;
    XMLDocument.load(FileName);
    if (XMLDocument.parseError.errorCode <> 0) then
    begin
      // MsgBox('The XML file could not be parsed. ' + XMLDocument.parseError.reason, mbError, MB_OK)
      Result := true;
    end
      else
    begin
      XMLDocument.setProperty('SelectionLanguage', 'XPath');
      XMLNodeList := XMLDocument.SelectNodes('/list_process/process');
      for Index := 0 to XMLNodeList.length - 1 do
      begin
        XMLNode := XMLNodeList.item[Index];
        if(Index = XMLNodeList.length - 1) then
        begin
          ContentMessage := ContentMessage + XMLNode.SelectSingleNode('title').Text + '.';
        end
          else
        begin
          ContentMessage := ContentMessage + XMLNode.SelectSingleNode('title').Text + ', ';
        end
      end;
      if XMLNodeList.length > 0 then
      begin
        // Ask the user a Yes/No question, defaulting to No
        if MsgBox(Format(CustomMessage('CustomUninstallAppRunningError'), [ContentMessage]), mbConfirmation, MB_YESNO or MB_DEFBUTTON2) = IDYES then
        begin
          // Close all application
          for Index := 0 to XMLNodeList.length - 1 do
          begin
               XMLNode := XMLNodeList.item[Index];
               if(not VarIsClear(XMLNode.SelectSingleNode('pid').Text)) then
                   Exec('taskkill','/F /PID ' + XMLNode.SelectSingleNode('pid').Text, '',SW_HIDE,ewWaitUntilTerminated, ResultCode);
          end   
        end
          else
        begin
           DeleteFile(FileName);
           Result := false;
        end
      end
    end;
  except
    // MsgBox('An error occured!' + #13#10 + GetExceptionMessage, mbError, MB_OK);
    Result := true;
  end;
end;






PrevFirstButtonClick: TNotifyEvent;
procedure DirSelectButtonClick(Sender: TObject);
var
  PrevValue: string;
begin
  PrevValue := WizardForm.DirEdit.Text;

  { Call remembered handler }
  PrevFirstButtonClick(Sender);

  if WizardForm.DirEdit.Text <> PrevValue then
  begin
    { And do whatever you want to do when the value changes }
    MsgBox(Format('Value changed from "%s" to "%s".', [PrevValue, WizardForm.DirEdit.Text]),
      mbInformation, MB_OK);
  end;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if(CurPageID = wpSelectDir) then
  begin
    { Remember the standard handler }
    PrevFirstButtonClick := WizardForm.DirBrowseButton.OnClick;
    { And assign our override }
    // https://stackoverflow.com/questions/30728195/how-to-disable-browse-button-on-defaultdirectory-wizard-page   
    WizardForm.DirBrowseButton.OnClick := @DirSelectButtonClick;
  end
end;



