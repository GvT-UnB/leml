public class JavaStringArrayLengthExample {
       
        public static void main(String args[]){
               
                //create String array
                String[] strArray = new String[]{"Java", "String", "Array", "Length"};
               
                /*
                 * To get length of array, use length property of array.
                 */
                int length = strArray.length;
               
                System.out.println("String array length is: " + length);
               
                //print elements of an array
                for(int i=0; i < length; i++){
                        System.out.println(strArray[i]);
                }
        }
}