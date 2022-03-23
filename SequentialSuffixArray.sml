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
