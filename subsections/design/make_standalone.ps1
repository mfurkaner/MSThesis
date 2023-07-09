Set-Location .\cavity
.\make.ps1

Set-Location ..


$end = (Get-Content -Path common\common_end_design.tex)
$cavity = (Get-Content -Path cavity\out)

$np = "\newpage" + [System.Environment]::NewLine

Remove-Item -recurse .\*\ -exclude *.tex,*.ps1

$design = ($cavity + $np + $end)

Copy-Item "common\common_start_design.tex" -Destination "..\\design.tex"

($design) | Add-Content -Path ..\design.tex
pdflatex ..\design.tex -o ..\design.pdf