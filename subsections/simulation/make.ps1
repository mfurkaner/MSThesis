Set-Location .\poc
.\make.ps1
Set-Location ..\intermediate_versions
.\make.ps1

Set-Location ..

$start = (Get-Content -Path common\common_start_sim.tex)
$poc = (Get-Content -Path poc\out)
$int = (Get-Content -Path intermediate_versions\out)

$np = "\newpage" + [System.Environment]::NewLine

Remove-Item -recurse .\*\ -exclude *.tex,*.ps1

$design = ($start + $poc + $np + $int)

$out = ""

$save = 0

$replace = New-Object Collections.Generic.List[string]
$to = New-Object Collections.Generic.List[string]

foreach  ($line in $design.Split([System.Environment]::NewLine)){
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