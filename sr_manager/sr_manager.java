//frame code for programming assignment 6.
//file name must be same as the main class name, i.e., sr_manager.java
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
			p1.start();
			p2.start();
			p3.start();
			p4.start();
			p5.start();
			p6.start();
		}
		catch (RuntimeException e)
		{
			System.out.println("Error");
		}
		
	}
}

class sr1 // class for shared resource r1
{
	private static int avail1 = 3;

	// synchronized method cannot be interrupted
	// only one thread can access at a time
	public synchronized void acquire(int id) 
	{
		if(avail1 == 0)
		{
			try 
			{
				System.out.println("process " + id + " is waiting for sr1");
				this.wait();
			}
			catch (InterruptedException e)
			{
				System.out.println("Error");
			}
		}
		System.out.println("process " + id + " acquires sr1");
		avail1 -= 1;
	}

	// synchronized method cannot be interrupted
	// only one thread can access at a time
	public synchronized void release(int id) 
	{
		avail1 += 1;
		if(avail1 == 1)
		{
			try 
			{
				this.notify();
			}
			catch (RuntimeException e)
			{
				System.out.println("Error");
			}
		}
		System.out.println("process " + id + " releases sr1");
	}
}// class sr1

class sr2 // class for shared resource r2
{
	private static int avail2 = 2;

	// synchronized method cannot be interrupted
	// only one thread can access at a time
	public synchronized void acquire(int id) 
	{
		if(avail2 == 0)
		{
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
		avail2 += 1;
		if(avail2 == 1)
		{
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
}// class sr2

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
			this.sleep(1);
			r1.acquire(this.id);
			this.sleep(1);;
			r2.acquire(this.id);
			// display "process_i is working"
			System.out.println("Process " + this.id + " is working");
			// release r1 and r2
			this.sleep(1);
			r1.release(this.id);
			this.sleep(1);
			r2.release(this.id);
			
			System.out.println("process " + this.id + " has finished");
		}
		catch(InterruptedException e)
		{
			System.out.println("error");
		}
	}
}// class process