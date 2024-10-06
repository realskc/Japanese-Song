$inputDir = "cleaned_lyrics"
$outputDir = "parsed_lyrics"
$mecabPath = "D:\Program Files (x86)\MeCab\bin\mecab.exe"

if (-not (Test-Path $outputDir)) {
	New-Item -ItemType Directory -Path $outputDir
}

Get-ChildItem "$inputDir\*.txt" | ForEach-Object {
	$inputFile = $_.FullName
	$outputFile = Join-Path $outputDir $_.Name
	& $mecabPath $inputFile -o $outputFile
}
