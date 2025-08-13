# Sets volume to 100 and then says a message
$wshShell = new-object -com wscript.shell;1..50 | % {$wshShell.SendKeys([char]174)};1..50 | % {$wshShell.SendKeys([char]175)}
Function Out-Speech($Text) { Add-Type -AssemblyName System.speech; $TTS = New-Object System.Speech.Synthesis.SpeechSynthesizer; $TTS.Speak($Text) };
Out-Speech "Deez Nuts ha ha ha gottem"
Start-Sleep 1;
Out-Speech "By the way you look great in your webcam kek w"
Exit