Set-Location .\theory
.\make.ps1
Set-Location ..\design
.\make.ps1

Set-Location ..


$end = (Get-Content -Path common\common_end.tex)
$theory = (Get-Content -Path theory\out)
$design = (Get-Content -Path design\out)

$np = "\newpage" + [System.Environment]::NewLine

$thesis = ($theory + $np + $design + $np + $end)

Copy-Item "common\common_start.tex" -Destination "..\\msthesis.tex"

($thesis) | Add-Content -Path ..\msthesis.tex
pdflatex ..\msthesis.tex -o ..\msthesis.pdf