// Mark Philipp
// 109941708
// CSCI 117
// 11-21-17
// file name must be same as the main class name, i.e., sr_manager.java
// run with > java -cp ./;. sr_manager

public class sr_manager // main class
{
	static sr1 r1 = new sr1();
	static sr2 r2 = new sr2();

	public static void main(String args[]) 
	{
		// create and start 6 threads for 6 concurrent processes
		process p1 = new process(1, r1, r2);
		process p2 = new process(2, r1, r2);
		process p3 = new process(3, r1, r2);
		process p4 = new process(4, r1, r2);
		process p5 = new process(5, r1, r2);
		process p6 = new process(6, r1, r2);
		try
		{
			// start all 6 threads and catch exceptions.
			p1.start();
			p2.start();
			p3.start();
			p4.start();
			p5.start();
			p6.start();
		}
		catch (RuntimeException e)
		{
			// if we catch a RunTimeexception, throw an error
			System.out.println("Error");
		}
		
		while(p1.isAlive() || p2.isAlive() || p3.isAlive() || p4.isAlive() || p5.isAlive() || p6.isAlive())
		{
			// loop until all threads have finished running.
			continue;
		}
		
		System.exit(0); // once all threads have finished. Exit program.
	}
}

class sr1 // class for shared resource r1
{
	private static int avail1 = 3; // start with 3 resources available

	// synchronized method cannot be interrupted
	// only one thread can access at a time
	public synchronized void acquire(int id) 
	{
		// this function will let a thread acquire a resource.
		// Will also tell other threads to wait if no resources are available.
		
		if(avail1 == 0)
		{
			// if there is nothing available, then we tell the thread to wait.
			try 
			{
				System.out.println("process " + id + " is waiting for sr1");
				this.wait(); // tell thread to wait.
			}
			catch (InterruptedException e)
			{
				// catch InterruptedException and print an error if we catch it.
				System.out.println("Error");
			}
		}
		System.out.println("process " + id + " acquires sr1");
		avail1 -= 1; // if we get here, we have > 0 resources. Decrement them by 1.
	}

	// synchronized method cannot be interrupted
	// only one thread can access at a time
	public synchronized void release(int id) 
	{
		// This function will release a resource and notify waiting threads if a resource
		// becomes avialable.
		
		avail1 += 1; // whenever resource is released, add 1 back into the available pool
		if(avail1 == 1)
		{
			// if we only have 1 resource then we know there are threads waiting in line. Notify them.
			try 
			{
				this.notify();
			}
			catch (RuntimeException e)
			{
				// catch exception
				System.out.println("Error");
			}
		}
		System.out.println("process " + id + " releases sr1");
	}
}

class sr2 // class for shared resource r2
{
	private static int avail2 = 2;

	// synchronized method cannot be interrupted
	// only one thread can access at a time
	public synchronized void acquire(int id) 
	{
		// this function will let a thread acquire a resource.
		// Will also tell other threads to wait if no resources are available.
		
		if(avail2 == 0)
		{
			// this function will let a thread acquire a resource.
			// Will also tell other threads to wait if no resources are available.
			try 
			{
				System.out.println("process " + id + " is waiting for sr2");
				this.wait();
			}
			catch (InterruptedException e)
			{
				System.out.println("Error");
			}
		}
		System.out.println("process " + id + " acquires sr2");
		avail2 -= 1;
	}

	// synchronized method cannot be interrupted
	// only one thread can access at a time
	public synchronized void release(int id) 
	{
		// This function will release a resource and notify waiting threads if a resource
		// becomes avialable.
		
		avail2 += 1; // whenever resource is released, add 1 back into the available pool
		
		if(avail2 == 1)
		{
			// whenever resource is released, add 1 back into the available pool
			try
			{
				this.notify();
			}
			catch (RuntimeException e)
			{
				System.out.println("Error");
			}
		}
		System.out.println("process " + id + " releases sr2");
	}
}

class process extends Thread // thread class name is "process"
{
	static sr1 r1;
	static sr2 r2;
	private int id;

	public process(int k, sr1 r1, sr2 r2) // constructor
	{
		this.r1 = r1;
		this.r2 = r2;
		this.id = k;
		System.out.println("Thread for process " + this.id + " created");
	}

	public void run() 
	{
		// acquire r1 and r2
		try
		{
			// Try and acquire resources from r1 and r2 .
			this.sleep(1); // I was having weird behavior when I didn't use sleep. This keeps the thread running stable before every acquire call.
			r1.acquire(this.id); // acquire sr1
			this.sleep(1);;
			r2.acquire(this.id); // acquire sr2
			
			System.out.println("Process " + this.id + " is working");
			
			// release r1 and r2
			this.sleep(1); 
			r1.release(this.id); // release sr1
			this.sleep(1);
			r2.release(this.id); // release sr2
			
			System.out.println("process " + this.id + " has finished");
		}
		catch(InterruptedException e)
		{
			// catch Interrupted Exceptions
			System.out.println("error");
		}
	}
}