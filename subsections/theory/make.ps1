Set-Location .\temel-theory
.\make.ps1
Set-Location ..\numericalmethods
.\make.ps1
Set-Location ..\simulation
.\make.ps1
Set-Location ..\rhodotron
.\make.ps1

Set-Location ..

$start = (Get-Content -Path common\common_start_theory.tex)
$end = (Get-Content -Path common\common_end_theory.tex)
$bib = (Get-Content -Path common\theory_bib.tex)
$temel_theory = (Get-Content -Path temel-theory\out)
$numerical = (Get-Content -Path numericalmethods\out)
$sim = (Get-Content -Path simulation\out)
$rhodo = (Get-Content -Path rhodotron\out)

$np = "\newpage" + [System.Environment]::NewLine

Remove-Item -recurse .\*\ -exclude *.tex,*.ps1

$theory = ($start + $temel_theory + $np + $rhodo + $np + $numerical + $np + $sim + $end)


$out = ""

$save = 0

$replace = New-Object Collections.Generic.List[string]
$to = New-Object Collections.Generic.List[string]

foreach  ($line in $theory.Split([System.Environment]::NewLine)){
    if($line -eq "%#####"){
        if($save -eq 0){
            $save = 1
        }
        else{
            $save = 0
        }
    }
    elseif($line -like "%----*"){
        $line=$line.Remove(0, 5)
        $replace.Add($line.Substring(0, $line.IndexOf('+')))
        $line=$line.Remove(0, $line.IndexOf('+'))
        $line=$line.Remove(0, 4)
        $to.Add($line)
    }
    else{
        if($save -eq 1){
            $count=0
            foreach($item in $replace){
                if($line -clike ("*" + $item + "*")){
                    $line=$line.Replace($item, $to[$count])
                }
                $count++;
            }
            
            $out += ($line + [System.Environment]::NewLine)
        }
    }
}

$out | Out-File -FilePath .\out