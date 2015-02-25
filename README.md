# QPwerShell
QPowerShell provides Qt C++ developers a simple way to interact with the Windows PowerShell CLI. It has only been tested and with PS 2.0 but should also with newer versions.
This is my first project ever released to the public. Please feel free to submit bugs, suggestions, or feature requests.
### Installation
Simply clone this repo and include all files in the src folder into your project.
### Usage
```
#include "QPowerShell.h"
// Initialize a new QPowerShell Object.
    QPowerShell *ps = new QPowerShell(this);
// Set the maximum number of allowed pipes (PowerShell Processes). Default = 5.
    ps->setMaxPipes(3);
// Connect to it's finished signal:
    QObject::connect(ps, &QPowerShell::pipeFinished, this, &SomeClass::someSlot);
// Create a new pipe. Pipes are owned by the PowerShell instance (ps).
// You can optionally set a description to keep track of which one is returning.
    QPowerShellPipeline *pipe1 = ps->newPipe();
    pipe->setDescription("Number One!!!111");
// Add your the commands you would like executed. You can chain commands, parameters, and statements but only in the order they would make sense. Example: Get-Host | Get-ChildItem throws an error and does not make sense. You must use valid PS comands.
// Statements will ALWAYS execute last and in the order they are added.
    pipe1->addCommand("Get-ChildItem")
            .addParam("-Path", "C:\\")
            .addCommand("Where-Object")
            .addParam("-FilterScript", "{$_.Extension -eq \".txt\"}")
            .addStatement("ECHO \"Well aren't you a Qt pie...\"")
            .addStatement("ECHO \";)\"");
// Run the command, listen for the finished signal.
    pipe1->invoke();
```
### Signal / Slots Signature
```
- void pipeFinished(QPowerShellPipeline *pipe, bool success, QPowerShellPipeline::QPSError error, const QByteArray &result);
```
### QPSError Struct
The signal will return a bool if the command ran successfully. If it did not, the error object is populated with the error source (QPowerShellErrorSrc) a friendly error message (errorMessage) and then the raw PS error (rawErrorMessage).
### Future Improvements with no ETA / order.
 - Queue commands and return when all have finished.
 - Result parser object / scheme.
 - Revert a command via Undo-Transaction call. 
 - Create streaming API to read live output from PS on a specific pipeline.
 - Make the code prettier.

### Special Thanks
* [Dale Buckley] - So many things I can no longer keep track.
* [Miha Čančula] - For answering my silly questions and providing helpful hints and guidance.

[Dale Buckley]:https://github.com/dlbuckley
[Miha Čančula]:https://github.com/Noughmad




