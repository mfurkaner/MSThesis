Set-Location .\temel-theory
.\make.ps1
Set-Location ..\numericalmethods
.\make.ps1
Set-Location ..\simulation
.\make.ps1
Set-Location ..\rhodotron
.\make.ps1

Set-Location ..


$end = (Get-Content -Path common\common_end_theory.tex)
$bib = (Get-Content -Path common\theory_bib.tex)
$temel_theory = (Get-Content -Path temel-theory\out)
$numerical = (Get-Content -Path numericalmethods\out)
$sim = (Get-Content -Path simulation\out)
$rhodo = (Get-Content -Path rhodotron\out)

Remove-Item -recurse .\*\ -exclude *.tex,*.ps1

$theory = ($temel_theory + $rhodo + $numerical + $sim + $bib + $end)

Copy-Item "common\common_start_theory.tex" -Destination "..\\..\\theory.tex"

($theory) | Add-Content -Path ..\..\theory.tex
pdflatex ..\..\theory.tex -o ..\..\theory.pdf