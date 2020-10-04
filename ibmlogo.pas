uses

  Crt,
  {$define usecrt}
  {$define useacpisfalse}
  classes, sysutils, strutils;


var
   Line1,Line2,Line3,Line4,Line5,Line6,Line7,Line8,Line9,Line10,Line11 : string;
   c : string;

Begin
{$ifdef usecrt}
{$ifdef useacpisfalse}
SetUseACP(False);
{$endif useacpisfalse}
{$endif usecrt}

TextColor(LightBlue);

c:=chr(219);

Line1:='   77777777777777  7777777777777777777     777777777777         777777777777';
Line2:='   77777777777777  7777777777777777777777  7777777777777       7777777777777';
Line3:='   77777777777777  77777777777777777777777 77777777777777     77777777777777';
Line4:='       777777          7777777     7777777     77777777777   77777777777';
Line5:='       777777          77777777777777777       777777777777 777777777777';
Line6:='       777777          77777777777777777       7777777 777777777  777777';
Line7:='       777777          7777777     7777777     7777777  7777777   777777';
Line8:='   77777777777777  77777777777777777777777  7777777777   77777    7777777777';
Line9:='   77777777777777  7777777777777777777777   7777777777    777     7777777777';
Line10:='   77777777777777  7777777777777777777      7777777777     7      7777777777';
Line11:='                           Personal Computer 5160 XT';


Line1 := StringReplace(Line1, '7', c, [ rfReplaceAll ]);
Line2 := StringReplace(Line2, '7', c, [ rfReplaceAll ]);
Line3 := StringReplace(Line3, '7', c, [ rfReplaceAll ]);
Line4 := StringReplace(Line4, '7', c, [ rfReplaceAll ]);
Line5 := StringReplace(Line5, '7', c, [ rfReplaceAll ]);
Line6 := StringReplace(Line6, '7', c, [ rfReplaceAll ]);
Line7 := StringReplace(Line7, '7', c, [ rfReplaceAll ]);
Line8 := StringReplace(Line8, '7', c, [ rfReplaceAll ]);
Line9 := StringReplace(Line9, '7', c, [ rfReplaceAll ]);
Line10 := StringReplace(Line10, '7', c, [ rfReplaceAll ]);
Line11 := StringReplace(Line11, '7', c, [ rfReplaceAll ]);

ClrScr;
writeln(Line1);
writeln('');
writeln(Line2);
writeln(Line3);
writeln('');
writeln(Line4);
writeln('');
writeln(Line5);
writeln('');
//writeln('');
writeln(Line6);
writeln('');
writeln(Line7);
writeln('');
writeln(Line8);
writeln(Line9);
writeln('');
writeln(Line10);
writeln('');
writeln(Line11);

readln();
end.