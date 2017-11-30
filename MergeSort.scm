(define (mergesort L)
  (cond ((null? L)  '())
        ((= 1 (length L)) L)
        ((= 2 (length L)) (mergelists (list (car L))(cdr L)))
        (else (mergelists (mergesort (car (split L)) ) 
                          (mergesort (car (cdr (split L))) ) 
              ))
  )
)

(define (mergelists L M)         ; assume L and M are sorted lists
   (cond ( (null? L) M)
         ( (null? M) L)
         ( (< (car L)(car M))
              (cons (car L) (mergelists (cdr L)M)))
         (else
              (cons (car M) (mergelists L (cdr M))))
   )
)


(define (length L)         ; # elements in a list
   (cond ( (null? L) 0)
         (else (+ (length (cdr L)) 1))
  )
)


(define (sub L start stop ctr)    ; extract elements start to stop into a list
   (cond ( (null? L) L)
         ( (< ctr start) (sub (cdr L) start stop (+ ctr 1)))
         ( (> ctr stop) '() )
         (else
            (cons (car L) (sub (cdr L) start stop (+ ctr 1)))
         )
   )
)

 (define (split L)                 ; split the list in half:
                                  ; returns ((first half)(second half))
    (let ((len (length L)))
       (cond ((= len 0) (list L L) )
             ((= len 1) (list L '() ))
             (else (list (sub L 1 (/ len 2) 1)(sub L (+(/ len 2)1) len 1)
                  )
             )
       )
    )
) 

#|

mergesort(a, p, r) // p = start index. r = end index. A is list
	if (p < r)
		q = (p + r) / 2 // midpoint
		mergeSort (a, p, q) // recurse to get left half
		mergesort (a, q+1, r) // get right half
		merge (a, p, q, r)// merge first last and middle point into one list. Also handles sorting
		

mergesort (L)
			if(length L >= 2)
				Split(L) //create L1 L2
				merge_sort (L1)
				merge_sort (L2)
				merge (L1, L2)
			else return L

merge_sort (x)
	if(x is null) return x;
	else if (x is length 1) return x;
	else
		return merge(merge_sort(x1), merge_sort (x2)) // x1 = (car (split x)) x2 = (cdr (split x))
		
merge (x, y)
	if (x is null) return y;
	else if ( y is null) return x;
	else if( x[1] <= y[1])
		(cons (car x) (merge (cdr x) y)) // x without 1st element
	else
		(cons (car y) (merge x, (cdr y))) // y without 1st element
		
Split (x)
	if (x is null) (cons '() '()) // (cons '() '())
	else if(length x == 1) (cons '1 '())
	else
		
(define (split x)
	(cond ((null? x) (cons '() '())))
	(else (= (length L) 1) (cons 'x '()))
	(else (cons ((cons (car x)) car (split(cddr x))) ((cons (cadr x) (cdr (split (cddr x)))))))
)
|#

; split -> ( ( xxxx ) yyy ) 
; car (split) = (xxxx)
; cdr (split) = (yyy)