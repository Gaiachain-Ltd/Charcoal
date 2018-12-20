#----------------------------------------------------------------------------------------
#
# MILO @ 2016
# Uploader script for Milo Code Database
#
# Dependency:
#       curl -> https://wiki.milosolutions.com/index.php/Gitlab_CI_runners#Windows_.231
#----------------------------------------------------------------------------------------
[CmdletBinding(PositionalBinding=$false)]
param(
    [Parameter(Mandatory=$true)]
    [string]$file, 
    [Parameter(Mandatory=$true)]
    [string]$server,
    [Parameter(Mandatory=$true)]
    [string]$repo,
    [Parameter(Mandatory=$true)]
    [string]$user,
    [Parameter(Mandatory=$true)]
    [string]$password
)

class ScriptArgs {
    static validate() {
        # check if $file exists
        if( !(Test-Path $script:file) ) {
        Write-Host "File not exists: $script:file" -foreground red
        exit
        }
    }
}

class Uploader {
    [string] $file;
    [string] $server;
    [string] $repo;
    [string] $user;
    [string] $password;

    [string] $token;
    [string] $uploadLink;
    [string] $operation;
    
    # basename( $this.file )
    [string] $filename;
    
    # path of curl
    [string] $curl = "C:\Tools\curl\bin\curl.exe";

    # Constructor
    Uploader([string] $file, [string] $server, [string] $repo, [string] $user, [string] $password) {
        $this.file     = $file;
        $this.server   = $server;
        $this.repo     = $repo;
        $this.user     = $user;
        $this.password = $password;
        
        $this.isInstalled($this.curl);
        $this.getToken();
        $this.getLink();
    }

    [void] getToken() {
        $this.token = $( & $($this.curl) -d "username=$($this.user)&password=$($this.password)" "$($this.server)/api2/auth-token/" )
        if( !$this.token.Contains("token") ) {
            Write-Host "Could not get token. Server answer: $($this.token)" -foreground red
            exit
        }
        
        # retrieve from token only id number
        $this.token = $this.token.substring(11, $this.token.Length - 13);
    }

    # check whether $app is installed
    [void] isInstalled($app) {
        if( (Get-Command $app -ErrorAction SilentlyContinue) -eq $null ) {
            Write-Host "Cannot run $app. Check if it is installed and whether path is valid." -foreground red
            exit
        }
    }

    [void] getLink() {
        $files = $( & $($this.curl) -H "Authorization: Token $($this.token)" -H 'Accept: application/json; indent=4' "$($this.server)/api2/repos/$($this.repo)/dir/?p=/$($this.directory)" )
        
        # basename equivalent
        $this.filename = (Get-Item $this.file).Name;
        $this.operation = "upload-link";
        if( $files.Contains($this.filename) ) {
            $this.operation = "update-link";
        }
        
        Write-Host "Operation: $($this.operation)"
        $this.uploadLink = $( & $($this.curl) -H "Authorization: Token $($this.token)" "$($this.server)/api2/repos/$($this.repo)/$($this.operation)/" );
        
        # remove quotes
        $this.uploadLink = $this.uploadLink.Replace('"',"");
    }

    [void] run() {
        Write-Host "Run uploader..."
        if( $this.operation -eq "update-link" ) {
            Write-Host "Updating $($this.file)"
            & $($this.curl) -H "Authorization: Token $($this.token)" -F file=@$($this.file) -F filename=$($this.filename) -F "target_file=/$($this.directory)/$($this.filename)" "$($this.uploadLink)"
        }
        else {
            Write-Host "Uploading $($this.file)"
            & $($this.curl) -H "Authorization: Token $($this.token)" -F file=@$($this.file) -F filename=$($this.filename) -F "parent_dir=/$($this.directory)" "$($this.uploadLink)"
        }
    }
}

function main() {
    
    # validate script args
    [ScriptArgs]::validate();
    
    # run uploader with script args
    [Uploader]::new($file, $server, $repo, $user, $password).run();
}

# main
main;