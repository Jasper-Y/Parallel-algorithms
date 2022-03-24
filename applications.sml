fun constructlcp s = 
  let
    val n = List.length s
    val arr = Array.fromList s
    open Array
    val (suff, a) = suffixArrayOkay s
    val lcp = Array.array(n - 1, 0)
    fun loop i k = if i = n then lcp 
      else if sub(a, i) = n - 1 then loop (i + 1) 0
      else let
        val j = sub(suff, sub(a, i) + 1)
        fun scan k = if i + k < n andalso j + k < n andalso sub(arr, i + k) = sub(arr, j + k) then scan (k + 1) else k
        val k' = scan k
        val _ = update(lcp, sub(a, i), k')
      in
        loop (i + 1) (Int.max(k' - 1, 0))
      end
  in
    loop 0 0
  end