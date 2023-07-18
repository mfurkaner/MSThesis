Set-Location .\final_design
.\make.ps1
Set-Location ..\technical_drawings
.\make.ps1
Set-Location ..\manifacturing
.\make.ps1
Set-Location ..\rf
.\make.ps1

Set-Location ..

$start = (Get-Content -Path common\common_start_prod.tex)
$finald = (Get-Content -Path final_design\out)
$techd = (Get-Content -Path technical_drawings\out)
$manif = (Get-Content -Path manifacturing\out)
$rf = (Get-Content -Path rf\out)

$np = "\newpage" + [System.Environment]::NewLine

Remove-Item -recurse .\*\ -exclude *.tex,*.ps1

$prod = ($start + $finald + $np + $techd + $np + $manif + $np + $rf)

$out = ""

$save = 0

$replace = New-Object Collections.Generic.List[string]
$to = New-Object Collections.Generic.List[string]

foreach  ($line in $prod.Split([System.Environment]::NewLine)){
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