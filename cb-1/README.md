# Übungsblatt 1
## Generelles
Die ersten beiden Aufgaben dienen zur Vertiefung Ihrer praktischen Erfahrungen in C. Sie implementieren einen Stack und eine Baumstruktur, die wir später in abgewandelter Form bei der internen Repräsentation des eingelesenen Quelltextes sowie bei der Ausführung durch den Interpreter benötigen.

## Allgemeine Hinweise
Für diese und alle folgenden Praktikumsaufgaben gilt, dass Einsendungen, die in der jeweils mitgegebenen Testumgebung nicht laufen, mit null Punkten bewertet werden! Das beinhaltet insbesondere alle Programme, die sich nicht fehlerfrei kompilieren lassen. Als Testsystem werden wir dabei gruenau6 mit den dort installierten Compilern und Compilerwerkzeugen benutzen. Prüfen Sie bitte rechtzeitig vor der Abgabe, ob ihr Programm auch dort lauffähig ist. Wir akzeptieren keine Entschuldigungen der Form „aber bei mir Zuhause hat es funktioniert“ ;).

Ebenfalls mit null Punkten werden alle Abgaben bewertet, die sich nicht exakt an die vorgegebenen Formate halten.

> Um Ihnen die Abgabe zu erleichtern, geben wir Ihnen ein Makefile mit, welches die folgenden Ziele unterstützt:
> #### all
> Übersetzt die Quelldateien und erzeugt eine ausführbare Datei.
> #### run
> Übersetzt die Quelldateien, erzeugt eine ausführbare Datei und startet das Testprogramm.
> #### clean
> Entfernt alle Zwischendateien und räumt in Ihrem Verzeichnis auf.
> Bitte achten Sie bei Ihrer Implementation auf Speicherleckfreiheit bei korrekter Benutzung, d.h. bei paarweisem Aufruf von init() und release().

## Abgabemodus
Die Lösung ist in einem Git-Repository abzugeben. 

Zur Lösung der Aufgaben steht für Sie dieses Repository mit 
- vorgegebenen Header-Dateien stack.h und syntree.h, 
- den beiden Test-Dateien teststack.c und testsyntree.c 
- sowie Makefiles 

zur Verfügung.

## Aufgabe 1 (30 Punkte)
### Kurzbeschreibung
Implementieren Sie einen Stack, der beliebig viele Integerzahlen speichern kann ("beliebig viel" bedeutet für uns: lediglich begrenzt durch den Arbeitsspeicher). Erweitern Sie dabei eine vorgegebene Schnittstelle, so dass Ihr Stack in andere (bereits existierende) Programme eingebunden werden kann.

### Aufgabenstellung
Wie Sie in der gegebenen Headerdatei stack.h sehen können, betrachten wir einen Stack als eine Datenstruktur, auf der folgende Operationen ausgeführt werden können:

```c
// Initialisiert einen neuen Stack und liefert den Rückgabewert 0, 
// falls keine Fehler bei der Initialisierung aufgetreten sind und andernfalls einen Fehlercode.
int stackInit(IntStack *self)

// Gibt den Stack und alle assoziierten Strukturen frei.
void stackRelease(IntStack *self)

// Legt den Wert von i auf den Stack.
void stackPush(IntStack *self, int i)

// Gibt das oberste Element des Stacks zurück.
int stackTop(const IntStack *self)

// Entfernt und liefert das oberste Element des Stacks.
int stackPop(IntStack *self)

// Gibt zurück, ob ein Stack leer ist, d.h. kein Element enthält (Rückgabewert != 0, wenn leer; == 0, wenn nicht). 
int stackIsEmpty(const IntStack *self)
```

- Implementieren Sie den Stack in der Datei stack.c und erweitern Sie die Schnittstelle stack.h, so dass ein externes Programm nur die Headerdatei einbinden muss, um mit Ihren Stacks arbeiten zu können.

- Sollte beim Aufruf einer der Methoden ein Fehler auftauchen, so soll eine sinnvolle Fehlerausgabe nach stderr und der Abbruch des Programms mit Exitcode != 0 erfolgen.

- Zusätzlich stellen wir Ihnen ein Makefile und ein Testprogramm in der Datei teststack.c zur Verfügung. Ihre Lösung befindet sich auf dem richtigen Weg, wenn dieses Testprogramm (ohne Änderungen an den beiden letztgenannten Dateien) erfolgreich kompiliert und durchläuft. Bitte beachten Sie jedoch, dass die Testumgebung bei weitem nicht alle interessanten Testfälle abdeckt.

- Hinweis: Sie werden feststellen, dass in der Datei stack.h eine Funktion namens void `stackPrint(const IntStack *self)` deklariert wurde. Es steht Ihnen frei diese Funktion zu Testzwecken zu implementieren.

## Aufgabe 2 (70 Punkte)
### Kurzbeschreibung
Implementieren Sie eine Datenstruktur, die beliebig verzweigte Bäume speichern kann, mit den im Folgenden beschriebenen Methoden.

### Aufgabenstellung
Die hier von Ihnen zu implementierende Datenstruktur werden Sie bei der Konstruktion des abstrakten Syntaxbaumes benötigen, da uns die Bordmittel von C keine wirkliche Hilfe leisten.

- Die zu implementierende Datenstruktur Syntree soll Baumknoten in beliebig komplexen Konfigurationen speichern können und davon beliebig viele. 
- Beschreiben Sie die Implementation Ihrer Datenstruktur kurz in einem Programmkommentar am Anfang der Headerdatei.
- Implementieren Sie dann die folgenden Operationen:

```c
// Initialisiert einen neuen Syntaxbaum und liefert den Rückgabewert 0, falls keine Fehler bei der 
// Initialisierung aufgetreten sind und andernfalls einen Fehlercode.
int syntreeInit(Syntree *self)

// Gibt den Syntaxbaum und alle damit assoziierten Strukturen frei.
void syntreeRelease(Syntree *self)

// Erstellt einen neuen Knoten mit einem Zahlenwert als Inhalt und gibt dessen ID zurück; 
// der ID-Typ ist durch Sie Ihrer Implementation entsprechend festzulegen.
SyntreeNodeID syntreeNodeNumber(Syntree *self, int number)

// Kapselt einen Knoten innerhalb eines anderen Knotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodeTag(Syntree *self, SyntreeNodeID id)

// Kapselt ein Knotenpaar innerhalb eines Knotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodePair(Syntree *self, SyntreeNodeID id1, SyntreeNodeID id2)

// Hängt einen Knoten an das Ende eines Listenknotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodeAppend(Syntree *self, SyntreeNodeID list, SyntreeNodeID elem)

// Hängt einen Knoten an den Anfang eines Listenknotens und gibt dessen ID zurück.
SyntreeNodeID syntreeNodePrepend(Syntree *self, SyntreeNodeID elem, SyntreeNodeID list)

// Gibt alle Zahlenknoten zwischen runden und alle zusammengesetzten Knoten zwischen 
// geschweiften Klammern rekursiv (depth-first) auf der Standardausgabe aus.
void syntreePrint(const Syntree *self, SyntreeNodeID root)
```

- Hinweis: es ist erlaubt bei der Ausgabe beliebig viele Leerzeichen und Zeilenenden zu verwenden; wir werden alle Zeichen aus der Ausgabe entfernen, für die isspace() wahr ist.
- Sollte beim Aufruf einer der Methoden ein Fehler auftauchen, so soll eine sinnvolle Fehlerausgabe nach stderr und der Abbruch des Programms mit Exitcode != 0 erfolgen.
- Führen Sie die Implementation in der Datei syntree.c durch. Zur Selbstüberprüfung stellen wir Ihnen auch hier ein Beispielprogramm und ein Makefile zur Verfügung.