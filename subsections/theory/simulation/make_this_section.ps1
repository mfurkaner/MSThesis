$total = (Get-Content -Path simulation.tex)

$temel_theory = ""

$save = 0

foreach  ($line in $total.Split([System.Environment]::NewLine)){
    if($line -eq "%%%%%%"){
        if($save -eq 0){
            $save = 1
        }
        else{
            $save = 0
        }
    }
    else{
        if($save -eq 1){
            $temel_theory += ($line + [System.Environment]::NewLine)
        }
    }
}

$temel_theory | Out-File -FilePath .\sim