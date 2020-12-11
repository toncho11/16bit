uses
  Crt,
  sysutils, strutils;
  //classes won't work on 8086

var
   Line1,Line2,Line3,Line4,Line5,Line6,Line7,Line8,Line9,Line10,Line11 : string;
   c : Char;
   i : integer;

Begin

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

for i := length(Line1) downto 1 do
       if Line1[i] = '7' then Line1[i] := c;
for i := length(Line2) downto 1 do
       if Line2[i] = '7'  then Line2[i] := c;
for i := length(Line3) downto 1 do
       if Line3[i] = '7'  then Line3[i] := c;
for i := length(Line4) downto 1 do
       if Line4[i] = '7'  then Line4[i] := c;
for i := length(Line5) downto 1 do
       if Line5[i] = '7'  then Line5[i] := c;
for i := length(Line6) downto 1 do
       if Line6[i] = '7'  then Line6[i] := c;
for i := length(Line7) downto 1 do
       if Line7[i] = '7'  then Line7[i] := c;
for i := length(Line8) downto 1 do
       if Line8[i] = '7'  then Line8[i] := c;
for i := length(Line9) downto 1 do
       if Line9[i] = '7'  then Line9[i] := c;
for i := length(Line10) downto 1 do
       if Line10[i] = '7'  then Line10[i] := c;
for i := length(Line11) downto 1 do
       if Line11[i] = '7'  then Line11[i] := c;
	   
//ClrScr;
writeln(Line1);
writeln('');
writeln(Line2);
writeln(Line3);
writeln('');
writeln(Line4);
writeln('');
writeln(Line5);
writeln('');
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

TextColor(White);

end.