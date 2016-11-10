#HW 1

## CSD 304 - Computer Networks

## Submitted by: Rohan Verma (1510110508

### 36 The Unix utility ping can be used to find the RTT to various Internet hosts. Read the man page for ping, and use it to find the RTT to www.cs.princeton.edu in New Jersey and www.cisco.com in California. Measure the RTT values at different times of day, and compare the results. What do you think accounts for the differences?

#### Princeton results

The pings timed out. 

#### Cisco.com results

Results of ping with rtt

**Morning**

```bash
ping www.cisco.com -c 200

200 packets transmitted, 200 received, 0% packet loss, time 174007ms
rtt min/avg/max/mdev = 25.477/31.547/55.129/11.792 ms

```

**Evening**

```bash
ping www.cisco.com -c 200

200 packets transmitted, 200 received, 0% packet loss, time 175290ms
rtt min/avg/max/mdev = 25.017/34.321/286.826/26.407 ms
```

**Night**

```bash
ping www.cisco.com -c 200

200 packets transmitted, 200 received, 0% packet loss, time 175220ms
rtt min/avg/max/mdev = 25.007/35.391/76.726/24.407 ms
```

**IP that was being hit**: `a23-57-240-170.deploy.static.akamaitechnologies.com (23.57.240.170)`

**Whois **

```bash
OrgName:        Akamai Technologies, Inc.
OrgId:          AKAMAI
Address:        8 Cambridge Center
City:           Cambridge
StateProv:      MA
```

####Reasons due to which RTT value difference can be affected [1]:

1. Time of day / Host load - This can be a factor due to the server load.
2. Geographical Location - I pinged IITDelhi since it is geographically closer. It is clearly a smaller RTT.

```bash 
ping www.iitd.ac.in -c 20

--- www.iitd.ac.in ping statistics ---
20 packets transmitted, 20 received, 0% packet loss, time 19031ms
rtt min/avg/max/mdev = 7.025/7.814/10.350/0.791 ms

```



3. Local network usage - Since, SNU routers are less used during the morning hours RTT was observed to be lower for cisco.
4. RTT is also being affected since the ping seems to be hitting a content delivery network to fetch the data instead of the actual cisco server.




### 37. The Unix utility traceroute, or its Windows equivalent tracert, can be used to find the sequence of routers through which a message is routed. Use this to find the path from your site to some others. How well does the number of hops correlate with the RTT times from ping? How well does the number of hops correlate with geographical distance?

#### Princeton Traceroute

```bash
traceroute to www.cs.princeton.edu (128.112.136.35), 30 hops max, 60 byte packets
 1  gateway (10.6.15.254)  1.231 ms  1.556 ms  1.943 ms
 2  10.1.15.251 (10.1.15.251)  4.551 ms  4.490 ms  4.459 ms
 3  10.12.0.1 (10.12.0.1)  4.429 ms  4.463 ms  4.741 ms
 4  10.1.16.1 (10.1.16.1)  7.239 ms  7.196 ms  7.033 ms
 5  103.27.164.1 (103.27.164.1)  7.072 ms  7.030 ms  7.404 ms
 6  * * *
 7  * * *
 8  62.216.147.73 (62.216.147.73)  28.015 ms  25.906 ms  27.744 ms
 9  * * *
10  xe-4-2-0.0.cji01.ldn004.flagtel.com (62.216.129.138)  232.851 ms xe-5-2-0.0.cji01.ldn004.flagtel.com (62.216.128.114)  147.114 ms  153.304 ms
11  40ge1-3.core1.lon2.he.net (195.66.224.21)  165.520 ms  166.944 ms  151.230 ms
12  100ge1-1.core1.nyc4.he.net (72.52.92.166)  218.657 ms  224.336 ms  220.070 ms
13  princeton-university.10gigabitethernet1-1-6.switch1.nyc8.he.net (216.66.49.74)  225.394 ms  223.734 ms  216.850 ms
14  core-87-router.princeton.edu (128.112.12.130)  216.112 ms  221.719 ms  268.720 ms
15  csgate.princeton.edu (128.112.12.58)  222.858 ms  221.267 ms  214.702 ms
16  * * *
17  * * *
18  * * *
19  * * *
20  * * *
21  * * *
22  * * *
23  * * *
24  * * *
25  * * *
26  * * *
27  * * *
28  * * *
29  * * *
30  * * *
```

#### Cisco Traceroute

```bash 
traceroute to www.cisco.com (23.57.240.170), 30 hops max, 60 byte packets
 1  gateway (10.6.15.254)  8.009 ms  9.559 ms  12.962 ms
 2  10.1.15.251 (10.1.15.251)  14.318 ms  16.894 ms  17.795 ms
 3  10.12.0.1 (10.12.0.1)  21.576 ms  20.328 ms  19.051 ms
 4  10.1.16.1 (10.1.16.1)  23.654 ms  22.741 ms  26.675 ms
 5  103.27.164.1 (103.27.164.1)  24.476 ms  27.881 ms  25.419 ms
 6  * * *
 7  * * *
 8  115.248.111.25 (115.248.111.25)  34.741 ms  34.215 ms  34.458 ms
 9  59.163.25.241.static.vsnl.net.in (59.163.25.241)  32.049 ms  31.891 ms  31.650 ms
10  * * *
11  14.140.113.118.static-delhi-vsnl.net.in (14.140.113.118)  31.360 ms  31.929 ms  31.650 ms
12  * * *
13  * * *
14  * * *
15  * * *
16  * * *
17  * * *
18  * * *
19  * * *
20  * * *
21  * * *
22  * * *
23  * * *
24  * * *
25  * * *
26  * * *
27  * * *
28  * * *
29  * * *
30  * * *
```

###But in unix systems traceroute uses UDP instead of ICMP by default so I was not sure if we got good results since UDP might be blocked by some server like at 14.140.113.118.static-delhi-vsnl.net.in so I decided to try with ICMP

I also learned that in practice, this seems to make little difference in the outcome, unless a system along the route is blocking one type of traffic but not the other.

####Princeton

```bash
sudo traceroute www.cs.princeton.edu -I
traceroute to www.cs.princeton.edu (128.112.136.35), 30 hops max, 60 byte packets
 1  * * *
 2  10.1.0.10 (10.1.0.10)  1.301 ms  1.286 ms  0.993 ms
 3  10.1.16.1 (10.1.16.1)  1.559 ms  1.571 ms  1.572 ms
 4  103.27.164.1 (103.27.164.1)  1.572 ms  2.042 ms  2.050 ms
 5  * * *
 6  * * *
 7  62.216.147.73 (62.216.147.73)  30.663 ms  30.820 ms  30.808 ms
 8  * * *
 9  xe-5-2-0.0.cji01.ldn004.flagtel.com (62.216.128.114)  149.352 ms  151.779 ms  151.848 ms
10  40ge1-3.core1.lon2.he.net (195.66.224.21)  153.697 ms  153.428 ms  154.068 ms
11  100ge1-1.core1.nyc4.he.net (72.52.92.166)  228.056 ms  228.036 ms  227.856 ms
12  princeton-university.10gigabitethernet1-1-6.switch1.nyc8.he.net (216.66.49.74)  219.587 ms  220.713 ms  220.683 ms
13  core-87-router.princeton.edu (128.112.12.130)  221.820 ms  221.269 ms  221.695 ms
14  csgate.princeton.edu (128.112.12.58)  229.610 ms  229.777 ms  229.715 ms^C
```

##### Observed Path

India - London - New York - Princeton

####Cisco 

```bash 
sudo traceroute www.cisco.com -I
traceroute to www.cisco.com (23.57.240.170), 30 hops max, 60 byte packets
 1  gateway (10.13.4.254)  1.359 ms  1.489 ms  1.543 ms
 2  10.1.0.10 (10.1.0.10)  1.499 ms  1.460 ms  1.417 ms
 3  10.1.16.1 (10.1.16.1)  1.364 ms  1.341 ms  1.330 ms
 4  103.27.164.1 (103.27.164.1)  1.672 ms  1.954 ms  1.954 ms
 5  * * *
 6  * * *
 7  115.248.111.21 (115.248.111.21)  32.363 ms  32.378 ms  32.081 ms
 8  59.163.25.241.static.vsnl.net.in (59.163.25.241)  33.835 ms  34.044 ms  34.080 ms
 9  * * *
10  14.140.113.118.static-delhi-vsnl.net.in (14.140.113.118)  29.685 ms  29.772 ms  29.369 ms
11  a23-57-240-170.deploy.static.akamaitechnologies.com (23.57.240.170)  29.414 ms  29.793 ms  29.255 ms
```

#### What I learned about ***

Receiving a *** [2] does not necessarily indicate a problem; in fact, it is usually normal, and just means that the system discarded the packet for some reason. Many systems do this normally. These are most often computers, rather than dedicated routers. Systems running Solaris routinely show an asterisk instead of the second RTT. It's important to remember that timeouts are not necessarily an indication of packet loss.

##### Observed Path

SNU - New Delhi - Akamai CDN - Cisco

### 38 Use traceroute, above, to map out some of the routers within your organization (or to verify none is used).

 1  gateway (10.6.15.254)  8.009 ms  9.559 ms  12.962 ms
 2  10.1.15.251 (10.1.15.251)  14.318 ms  16.894 ms  17.795 ms
 3  10.12.0.1 (10.12.0.1)  21.576 ms  20.328 ms  19.051 ms
 4  10.1.16.1 (10.1.16.1)  23.654 ms  22.741 ms  26.675 ms

 1  gateway (10.6.15.254)  1.231 ms  1.556 ms  1.943 ms
 2  10.1.15.251 (10.1.15.251)  4.551 ms  4.490 ms  4.459 ms
 3  10.12.0.1 (10.12.0.1)  4.429 ms  4.463 ms  4.741 ms
 4  10.1.16.1 (10.1.16.1)  7.239 ms  7.196 ms  7.033 ms


## References: 
[1] : https://www3.cs.stonybrook.edu/~phillipa/papers/SPECTS.pdf

[2] : http://www.exit109.com/~jeremy/news/providers/traceroute.html
