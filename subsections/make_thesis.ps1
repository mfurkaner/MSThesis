Set-Location .\theory
.\make.ps1
Set-Location ..\design
.\make.ps1
Set-Location ..\simulation
.\make.ps1

Set-Location ..


$end = (Get-Content -Path common\common_end.tex)
$bib = (Get-Content -Path common\bib)
$app = (Get-Content -Path .\common\appendix)
$theory = (Get-Content -Path theory\out)
$design = (Get-Content -Path design\out)
$sim = (Get-Content -Path simulation\out)

$np = "\newpage" + [System.Environment]::NewLine

$thesis = ($theory + $np + $design + $np +$sim + $np + $app + $bib + $end)

Remove-Item .\*\out

Copy-Item "common\common_start.tex" -Destination "..\\msthesis.tex"

($thesis) | Add-Content -Path ..\msthesis.tex
Set-Location ..
pdflatex msthesis.tex --shell-escape
Start-Sleep -Seconds 10
pdflatex msthesis.tex --shell-escape
Remove-Item .\*.aux, .\*.fdb_latexmk,.\*.fls, .\*.log, .\*.toc
Set-Location .\subsections