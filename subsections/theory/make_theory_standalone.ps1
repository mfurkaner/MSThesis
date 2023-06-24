Set-Location .\temel-theory
.\make_this_section.ps1
Set-Location ..\numericalmethods
.\make_this_section.ps1
Set-Location ..\simulation
.\make_this_section.ps1
Set-Location ..\rhodotron
.\make_this_section.ps1

Set-Location ..


$end = (Get-Content -Path common_end_theory.tex)
$temel_theory = (Get-Content -Path temel-theory\temel-theory)
$numerical = (Get-Content -Path numericalmethods\numerical)
$sim = (Get-Content -Path simulation\sim)
$rhodo = (Get-Content -Path rhodotron\rhodo)


$theory = ($temel_theory + $rhodo + $numerical + $sim + $end)

Copy-Item "common_start_theory.tex" -Destination "..\\..\\theory.tex"

($theory) | Add-Content -Path ..\..\theory.tex
pdflatex ..\..\theory.tex -o ..\..\theory.pdf