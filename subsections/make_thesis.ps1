Set-Location .\theory
.\make.ps1
Set-Location ..\tools
.\make.ps1
Set-Location ..\design
.\make.ps1
Set-Location ..\simulation
.\make.ps1
Set-Location ..\production
.\make.ps1
Set-Location ..\future
.\make.ps1

Set-Location ..


$end = (Get-Content -Path common\common_end.tex)
$bib = (Get-Content -Path common\bib)

$bibtexbib = "\bibliographystyle{styles/fbe_tez_v11.bst} %Still may have problems\n
\bibliography{msthesis} %Still may have problems"
$app = (Get-Content -Path .\common\appendix)
$theory = (Get-Content -Path theory\out)
$tools = (Get-Content -Path tools\out)
$design = (Get-Content -Path design\out)
$sim = (Get-Content -Path simulation\out)
$prod = (Get-Content -Path production\out)
$future = (Get-Content -Path future\out)

$np = "\newpage" + [System.Environment]::NewLine

$thesis = ($theory + $np + $tools + $np + $design + $np + $sim + $np + $prod + $np + $future + $np + $bibtexbib + $app + $end)

Remove-Item .\*\out

Copy-Item "common\common_start.tex" -Destination "..\\msthesis.tex"

($thesis) | Add-Content -Path ..\msthesis.tex
Set-Location ..
#Remove-Item .\*.aux, .\*.fdb_latexmk,.\*.fls, .\*.log, .\*.toc, .\*.lot, .\*.lof
pdflatex msthesis.tex --shell-escape
bibtex msthesis
Start-Sleep -Seconds 6
pdflatex msthesis.tex --shell-escape
#Remove-Item .\*.aux, .\*.fdb_latexmk,.\*.fls, .\*.log, .\*.toc, .\*.lot, .\*.lof
Copy-Item msthesis.pdf msthesis_temp.pdf
Set-Location .\subsections