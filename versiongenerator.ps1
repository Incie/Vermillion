$StartDate=(GET-DATE)
$EndDate=[datetime]”01/01/2000 00:00”

$spandays=[int32](NEW-TIMESPAN –Start $StartDate –End $EndDate).Days 
$spanstring = 'const char* generatedVersionString = "' + -$spandays +'";';

$stream = [System.IO.StreamWriter] "Source/version.h"
$stream.WriteLine($spanstring)
$stream.close()