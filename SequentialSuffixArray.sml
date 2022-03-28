(* -------------------------- *)
(* UTIL CODE *)

infixr 0 $
fun f $ a = f a

signature RANGE = 
  sig
    type range
    
    exception EmptyRange
    exception Loop
    exception EmptyStep
    
    exception NestedRange
    
    val to : int * int -> range
    val until : int * int -> range 
    
    val by : range * int -> range
    
    val for : range -> (int -> 'a) -> 'a
    
    val iterate : (int * 'a -> 'a) -> 'a -> range -> 'a
  end

structure Range :> RANGE = 
  struct
    datatype range =   
      inc of int * int 
    | exc of int * int 
    | stepUp of range * int 
    | stepDown of range * int
    
    exception EmptyRange
    exception Loop
    exception EmptyStep
    
    exception NestedRange
    
    infix to
    infix until
    infix by
    
    fun x to y = inc(x, y)
    
    fun x until y = if x = y then raise EmptyRange else exc(x, y)
    
    fun r by x = 
      case r of
        stepUp(_) => raise NestedRange
      | stepDown(_) => raise NestedRange
      | _ => 
          if x = 0 then raise EmptyStep 
          else if x < 0 then stepDown(r, x)
          else stepUp(r, x)
            
    
    fun for r f = 
      case r of
        inc(x, y) => 
          if x = y then f x 
          else if x > y then raise Loop
          else (f x; for (inc(x + 1, y)) f)
      | exc(x, y) => 
          if x + 1 = y then f x 
          else if x >= y then raise Loop
          else (f x; for (exc(x + 1, y)) f)
      | stepUp(inc(x, y), i) => 
          if x > y then raise Loop
          else if x = y orelse x + i > y then f x
          else (f x; for (stepUp(inc(x + i, y), i)) f) 
      | stepDown(inc(x, y), i) =>
          if x < y then raise Loop
          else if x = y orelse x + i < y then f x 
          else (f x; for (stepDown(inc(x + i, y), i)) f)
      | stepUp(exc(x, y), i) => 
          if x >= y then raise Loop
          else if x + i >= y then f x 
          else (f x; for (stepUp(exc(x + i, y), i)) f)
      | stepDown(exc(x, y), i) =>
          if x <= y then raise Loop
          else if x + i <= y then f x 
          else (f x; for (stepDown(exc(x + i, y), i)) f)
      | _ => raise NestedRange
      
    fun iterate f b r = 
     case r of
        inc(x, y) => 
          if x = y then f (x, b) 
          else if x > y then raise Loop
          else iterate f (f (x, b)) (inc(x + 1, y)) 
      | exc(x, y) => 
          if x + 1 = y then f (x, b)
          else if x >= y then raise Loop
          else iterate f (f (x, b)) (exc(x + 1, y)) 
      | stepUp(inc(x, y), i) => 
          if x > y then raise Loop
          else if x = y orelse x + i > y then f (x, b)
          else iterate f (f(x, b)) (stepUp(inc(x + i, y), i)) 
      | stepDown(inc(x, y), i) =>
          if x < y then raise Loop
          else if x = y orelse x + i < y then f (x, b)
          else iterate f (f(x, b)) (stepDown(inc(x + i, y), i))
      | stepUp(exc(x, y), i) => 
          if x >= y then raise Loop
          else if x + i >= y then f (x, b)
          else iterate f (f(x, b)) (stepUp(exc(x + i, y), i))
      | stepDown(exc(x, y), i) =>
          if x <= y then raise Loop
          else if x + i <= y then f (x, b)
          else iterate f (f(x, b)) (stepDown(exc(x + i, y), i))
      | _ => raise NestedRange
  end
  
open Range

infix to
infix until
infix by

(* UTIL CODE END *)
(*-----------------------------*)



(* O(n^2logn) *)
fun suffixArrayHorrible s = 
  let
    val s = String.explode s
    val n = List.length s
    val (suffixes, _) = foldl (fn (a, (l, s)) => ((s, a)::l, tl s)) ([], s) (List.tabulate(n, fn i => i))
    val A = ListMergeSort.sort (fn ((s1, _), (s2, _)) => List.collate Char.compare (s1, s2) = GREATER) suffixes
  in
    Vector.fromList (map #2 A)
  end

(* O(nlog^2n) *)
fun suffixArrayBad s = 
  let
    open Array
    val a = fromList s
    val n = length a
    val w = array(n, ((0,0),0))
    fun compare ((a as (x1, y1), _), (b as (x2, y2), _)) = if x1 < x2 orelse (x1 = x2 andalso y1 < y2) then LESS else if a = b then EQUAL else GREATER
    fun pass shift = 
      let
        val _ = for (0 to n - 1)
          (fn i => update(w, i, ((sub(a, i), sub(a, i+shift) handle _ => ~1), i)))
        val _ = ArrayQSort.sort compare w
        val _ = update(a, #2 $ sub(w, 0), 0)
        val _ = for (1 to n - 1)
          (fn j => update(a, #2 $ sub(w, j), sub(a, #2 (sub(w, j - 1))) + (if #1 (sub(w, j - 1)) = #1 (sub(w, j)) then 0 else 1)))
      in
        if sub(a, #2 $ sub(w, n - 1)) < n - 1 then pass $ shift * 2 else ()
      end
    val _ = pass 1
  in
    Array.tabulate(n, fn i => #2 $ sub(w, i))
  end
  
(* O(nlogn) *)
fun suffixArrayOkay s = 
  let
    open Array
    val a = fromList s
    val n = length a
    val w = array(n, ((0,0),0))
    fun bucketSort A = 
      let
        val buckets = tabulate(n, fn _ => tabulate(n + 1, fn _ => []))
        val _ = app (fn x as ((i, j), _) => update(sub(buckets, i), j + 1, x::sub(sub(buckets, i), j + 1))) A
        val L = foldr (fn (b1, l1) => foldr (fn (b2, l2) => b2 @ l2) l1 b1) [] buckets
        val _ = List.foldl (fn (x, i) => i + 1 before update(A, i, x)) 0 L
      in
        ()
      end
    fun compare ((a as (x1, y1), _), (b as (x2, y2), _)) = if x1 < x2 orelse (x1 = x2 andalso y1 < y2) then LESS else if a = b then EQUAL else GREATER
    fun pass shift = 
      let
        val _ = for (0 to n - 1)
          (fn i => update(w, i, ((sub(a, i), sub(a, i+shift) handle _ => ~1), i)))
        val _ = if shift = 1 then ArrayQSort.sort compare w else bucketSort w
        val _ = update(a, #2 $ sub(w, 0), 0)
        val _ = for (1 to n - 1)
          (fn j => update(a, #2 $ sub(w, j), sub(a, #2 (sub(w, j - 1))) + (if #1 (sub(w, j - 1)) = #1 (sub(w, j)) then 0 else 1)))
      in
        if sub(a, #2 $ sub(w, n - 1)) < n - 1 then pass $ shift * 2 else ()
      end
    val _ = pass 1
  in
    Array.tabulate(n, fn i => #2 $ sub(w, i))
  end
  
  (* O(nlogn) *)
sstructure StringKey : ORD_KEY = 
  struct
    type ord_key = string
    val compare = String.compare
  end
structure RB = RedBlackMapFn(StringKey)
fun suffixArray s = let
  fun sort b l A = let
    fun ins(d, s, i) = RB.insert(d, s, i::(RB.lookup(d, s) handle _ => []))
    val d = foldl (fn (i, h) => ins (h, String.substring(s, i, l) handle _ => String.extract(s, i, NONE) , i)) RB.empty b  
    in foldl (fn ([x], r) => x::r | (v, r) => sort v (l * 2) r) A (RB.listItems d) end
  in List.rev (sort (List.tabulate(String.size s, fn i => i)) 1 []) end
