

#Intro
 The aim of this project is to build an opensource Java profiler using JVMTI interface. I am a performance engineer where I need to use several profiling tools to find the bottleneck. In market several java profiling tools are available( Jprofiler. dynatrace..), I am trying to build a tool like Jprofiler/dynatrace, which will be developed by open source community. If open sources enthusiast developers starts contributing on developing then this tool might be a big hit. I am requesting you please do contribute.

#Thanks-giving:

Thank you [JEREMY MANSON][1], your blog and source code helps a lot to understand this concept, and after reading your blog only I got this idea to develop an opensource tool. Though I have started developing this tool from scratch, you might find some code from Jermy's code base(specifically makefile).

[1]:http://jeremymanson.blogspot.in/

#Progress
 - Current code is able to collect jvm stack trace.
 - I have initiated of writing a client/server model to transfer the jvm stack trace to a remote client.
 - Remote client must has a GUI where we will provide JVM stat in a user friendly way, for easy understanding.

   
#Pre-requisites
 - g++11

#Contribute
 - Any interested geek, who believes in open-source can mail ( email.suman.roy@gmail.com) me, then we can discuss and carry forward this project.
