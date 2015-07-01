interface A {
  public void a();
}

class AImpl implements A {
    public void a() {
       System.out.println("I am AImpl");
    }
}

class AnotherAImpl implements A {
    public void a() {
       System.out.println("I am another AImpl");
    }
}

public class testeInvokeInterface {
	public static void main(String[] args){
	  A aa;
	  aa = new AImpl();
	  aa.a(); // prints I am AImpl
	  aa = new AnotherAImpl();
	  aa.a(); // now it prints I am another AImpl
	}
}