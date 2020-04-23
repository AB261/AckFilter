# Implementation of Ack Filtering
---
**Brief**: 

---

**Introduction**:



**Expected Advantages**: 



**Class Relationships, coupling and implementation details**: 

**References**:
* [**ns-3 code of FQ-CoDel**](https://gitlab.com/nsnam/ns-3-dev/blob/master/src/traffic-control/model/fq-codel-queue-disc.h) 
  
  _A flow queue used by the FqCoDel queue disc in ns3-dev_

* [**CAKE Technical Information**](https://www.bufferbloat.net/projects/codel/wiki/CakeTechnical/)
  
  _Details of set-associative hash_
 
* [**Piece of CAKE: A Comprehensive Queue Management Solution for Home Gateways**](https://arxiv.org/pdf/1804.07617.pdf)

  _This paper presents Common Applications Kept Enhanced (CAKE), a comprehensive network queue management system designed specifically for home Internet gateways_
  
**Testing**: 

> Commands to Run:
```shell
NS_LOG="FifoQueueDisc" ./waf --run "test-runner --suite=fifo-queue-disc"
NS_LOG="FifoQueueDisc" ./waf --run dumbbell
```

Test 3: This test is designed to verify the basic functionality of TCP ACK Filter.
We enqueued first a TCP packet with only ACK Flag enabled and some sequence
number, then we enqueued one TCP Packet with only ACK Flag enabled and
higher sequence number. The packet at HEAD of the queue will be dropped
since the loss of this ACK will not result in any information loss at TCP Sender
Side.