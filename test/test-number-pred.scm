;;  Filename : test-number-pred.scm
;;  About    : unit tests for number predicates
;;
;;  Copyright (C) 2006 YAMAMOTO Kengo <yamaken AT bp.iij4u.or.jp>
;;  Copyright (c) 2007 SigScheme Project <uim AT freedesktop.org>
;;
;;  All rights reserved.
;;
;;  Redistribution and use in source and binary forms, with or without
;;  modification, are permitted provided that the following conditions
;;  are met:
;;
;;  1. Redistributions of source code must retain the above copyright
;;     notice, this list of conditions and the following disclaimer.
;;  2. Redistributions in binary form must reproduce the above copyright
;;     notice, this list of conditions and the following disclaimer in the
;;     documentation and/or other materials provided with the distribution.
;;  3. Neither the name of authors nor the names of its contributors
;;     may be used to endorse or promote products derived from this software
;;     without specific prior written permission.
;;
;;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
;;  IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
;;  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;;  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
;;  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
;;  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
;;  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
;;  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
;;  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

(load "./test/unittest.scm")

(if (not (symbol-bound? 'number?))
    (test-skip "R5RS numbers is not enabled"))

(define tn test-name)


(tn "number?")
(assert-eq? (tn) #f (number? #f))
(assert-eq? (tn) #f (number? #t))
(assert-eq? (tn) #f (number? '()))
(if (provided? "sigscheme")
    (begin
      (assert-eq? (tn) #f (number? (eof)))
      (assert-eq? (tn) #f (number? (undef)))))

;; binary
(assert-eq? (tn) #t (number? #b0))
(assert-eq? (tn) #t (number? #b-0))
(assert-eq? (tn) #t (number? #b+0))
(assert-eq? (tn) #t (number? #b1))
(assert-eq? (tn) #t (number? #b-1))
(assert-eq? (tn) #t (number? #b+1))
;; octal
(assert-eq? (tn) #t (number? #o0))
(assert-eq? (tn) #t (number? #o-0))
(assert-eq? (tn) #t (number? #o+0))
(assert-eq? (tn) #t (number? #o1))
(assert-eq? (tn) #t (number? #o-1))
(assert-eq? (tn) #t (number? #o+1))
(assert-eq? (tn) #t (number? #o3))
(assert-eq? (tn) #t (number? #o-3))
(assert-eq? (tn) #t (number? #o+3))
;; decimal (implicit)
(assert-eq? (tn) #t (number? 0))
(assert-eq? (tn) #t (number? -0))
(assert-eq? (tn) #t (number? +0))
(assert-eq? (tn) #t (number? 1))
(assert-eq? (tn) #t (number? -1))
(assert-eq? (tn) #t (number? +1))
(assert-eq? (tn) #t (number? 3))
(assert-eq? (tn) #t (number? -3))
(assert-eq? (tn) #t (number? +3))
;; decimal (explicit)
(assert-eq? (tn) #t (number? #d0))
(assert-eq? (tn) #t (number? #d-0))
(assert-eq? (tn) #t (number? #d+0))
(assert-eq? (tn) #t (number? #d1))
(assert-eq? (tn) #t (number? #d-1))
(assert-eq? (tn) #t (number? #d+1))
(assert-eq? (tn) #t (number? #d3))
(assert-eq? (tn) #t (number? #d-3))
(assert-eq? (tn) #t (number? #d+3))
;; hexadecimal
(assert-eq? (tn) #t (number? #x0))
(assert-eq? (tn) #t (number? #x-0))
(assert-eq? (tn) #t (number? #x+0))
(assert-eq? (tn) #t (number? #x1))
(assert-eq? (tn) #t (number? #x-1))
(assert-eq? (tn) #t (number? #x+1))
(assert-eq? (tn) #t (number? #x3))
(assert-eq? (tn) #t (number? #x-3))
(assert-eq? (tn) #t (number? #x+3))
(assert-eq? (tn) #t (number? #xa))
(assert-eq? (tn) #t (number? #x-a))
(assert-eq? (tn) #t (number? #x+a))

(assert-eq? (tn) #f (number? 'symbol))
(assert-eq? (tn) #f (number? 'SYMBOL))
(assert-eq? (tn) #f (number? #\a))
(assert-eq? (tn) #f (number? #\あ))
(assert-eq? (tn) #f (number? ""))
(assert-eq? (tn) #f (number? " "))
(assert-eq? (tn) #f (number? "a"))
(assert-eq? (tn) #f (number? "A"))
(assert-eq? (tn) #f (number? "aBc12!"))
(assert-eq? (tn) #f (number? "あ"))
(assert-eq? (tn) #f (number? "あ0イう12!"))
(assert-eq? (tn) #f (number? +))
(assert-eq? (tn) #f (number? (lambda () #t)))

;; syntactic keywords should not be appeared as operand
(if sigscheme?
    (begin
      ;; pure syntactic keyword
      (assert-error (tn) (lambda () (number? else)))
      ;; expression keyword
      (assert-error (tn) (lambda () (number? do)))))

(call-with-current-continuation
 (lambda (k)
   (assert-eq? (tn) #f (number? k))))
(assert-eq? (tn) #f (number? (current-output-port)))
(assert-eq? (tn) #f (number? '(#t . #t)))
(assert-eq? (tn) #f (number? (cons #t #t)))
(assert-eq? (tn) #f (number? '(0 1 2)))
(assert-eq? (tn) #f (number? (list 0 1 2)))
(assert-eq? (tn) #f (number? '#()))
(assert-eq? (tn) #f (number? (vector)))
(assert-eq? (tn) #f (number? '#(0 1 2)))
(assert-eq? (tn) #f (number? (vector 0 1 2)))

(tn "integer?")
(assert-eq? (tn) #f (integer? #f))
(assert-eq? (tn) #f (integer? #t))
(assert-eq? (tn) #f (integer? '()))
(if (provided? "sigscheme")
    (begin
      (assert-eq? (tn) #f (integer? (eof)))
      (assert-eq? (tn) #f (integer? (undef)))))

;; binary
(assert-eq? (tn) #t (integer? #b0))
(assert-eq? (tn) #t (integer? #b-0))
(assert-eq? (tn) #t (integer? #b+0))
(assert-eq? (tn) #t (integer? #b1))
(assert-eq? (tn) #t (integer? #b-1))
(assert-eq? (tn) #t (integer? #b+1))
;; octal
(assert-eq? (tn) #t (integer? #o0))
(assert-eq? (tn) #t (integer? #o-0))
(assert-eq? (tn) #t (integer? #o+0))
(assert-eq? (tn) #t (integer? #o1))
(assert-eq? (tn) #t (integer? #o-1))
(assert-eq? (tn) #t (integer? #o+1))
(assert-eq? (tn) #t (integer? #o3))
(assert-eq? (tn) #t (integer? #o-3))
(assert-eq? (tn) #t (integer? #o+3))
;; decimal (implicit)
(assert-eq? (tn) #t (integer? 0))
(assert-eq? (tn) #t (integer? -0))
(assert-eq? (tn) #t (integer? +0))
(assert-eq? (tn) #t (integer? 1))
(assert-eq? (tn) #t (integer? -1))
(assert-eq? (tn) #t (integer? +1))
(assert-eq? (tn) #t (integer? 3))
(assert-eq? (tn) #t (integer? -3))
(assert-eq? (tn) #t (integer? +3))
;; decimal (explicit)
(assert-eq? (tn) #t (integer? #d0))
(assert-eq? (tn) #t (integer? #d-0))
(assert-eq? (tn) #t (integer? #d+0))
(assert-eq? (tn) #t (integer? #d1))
(assert-eq? (tn) #t (integer? #d-1))
(assert-eq? (tn) #t (integer? #d+1))
(assert-eq? (tn) #t (integer? #d3))
(assert-eq? (tn) #t (integer? #d-3))
(assert-eq? (tn) #t (integer? #d+3))
;; hexadecimal
(assert-eq? (tn) #t (integer? #x0))
(assert-eq? (tn) #t (integer? #x-0))
(assert-eq? (tn) #t (integer? #x+0))
(assert-eq? (tn) #t (integer? #x1))
(assert-eq? (tn) #t (integer? #x-1))
(assert-eq? (tn) #t (integer? #x+1))
(assert-eq? (tn) #t (integer? #x3))
(assert-eq? (tn) #t (integer? #x-3))
(assert-eq? (tn) #t (integer? #x+3))
(assert-eq? (tn) #t (integer? #xa))
(assert-eq? (tn) #t (integer? #x-a))
(assert-eq? (tn) #t (integer? #x+a))

(assert-eq? (tn) #f (integer? 'symbol))
(assert-eq? (tn) #f (integer? 'SYMBOL))
(assert-eq? (tn) #f (integer? #\a))
(assert-eq? (tn) #f (integer? #\あ))
(assert-eq? (tn) #f (integer? ""))
(assert-eq? (tn) #f (integer? " "))
(assert-eq? (tn) #f (integer? "a"))
(assert-eq? (tn) #f (integer? "A"))
(assert-eq? (tn) #f (integer? "aBc12!"))
(assert-eq? (tn) #f (integer? "あ"))
(assert-eq? (tn) #f (integer? "あ0イう12!"))
(assert-eq? (tn) #f (integer? +))
(assert-eq? (tn) #f (integer? (lambda () #t)))

;; syntactic keywords should not be appeared as operand
(if sigscheme?
    (begin
      ;; pure syntactic keyword
      (assert-error (tn) (lambda () (integer? else)))
      ;; expression keyword
      (assert-error (tn) (lambda () (integer? do)))))

(call-with-current-continuation
 (lambda (k)
   (assert-eq? (tn) #f (integer? k))))
(assert-eq? (tn) #f (integer? (current-output-port)))
(assert-eq? (tn) #f (integer? '(#t . #t)))
(assert-eq? (tn) #f (integer? (cons #t #t)))
(assert-eq? (tn) #f (integer? '(0 1 2)))
(assert-eq? (tn) #f (integer? (list 0 1 2)))
(assert-eq? (tn) #f (integer? '#()))
(assert-eq? (tn) #f (integer? (vector)))
(assert-eq? (tn) #f (integer? '#(0 1 2)))
(assert-eq? (tn) #f (integer? (vector 0 1 2)))

(tn "zero?")
(assert-error (tn) (lambda () (zero? #f)))
(assert-error (tn) (lambda () (zero? #t)))
(assert-error (tn) (lambda () (zero? '())))
(if (provided? "sigscheme")
    (begin
      (assert-error (tn) (lambda () (zero? (eof))))
      (assert-error (tn) (lambda () (zero? (undef))))))

;; binary
(assert-eq? (tn) #t (zero? #b0))
(assert-eq? (tn) #t (zero? #b-0))
(assert-eq? (tn) #t (zero? #b+0))
(assert-eq? (tn) #f (zero? #b1))
(assert-eq? (tn) #f (zero? #b-1))
(assert-eq? (tn) #f (zero? #b+1))
;; octal
(assert-eq? (tn) #t (zero? #o0))
(assert-eq? (tn) #t (zero? #o-0))
(assert-eq? (tn) #t (zero? #o+0))
(assert-eq? (tn) #f (zero? #o1))
(assert-eq? (tn) #f (zero? #o-1))
(assert-eq? (tn) #f (zero? #o+1))
(assert-eq? (tn) #f (zero? #o3))
(assert-eq? (tn) #f (zero? #o-3))
(assert-eq? (tn) #f (zero? #o+3))
;; decimal (implicit)
(assert-eq? (tn) #t (zero? 0))
(assert-eq? (tn) #t (zero? -0))
(assert-eq? (tn) #t (zero? +0))
(assert-eq? (tn) #f (zero? 1))
(assert-eq? (tn) #f (zero? -1))
(assert-eq? (tn) #f (zero? +1))
(assert-eq? (tn) #f (zero? 3))
(assert-eq? (tn) #f (zero? -3))
(assert-eq? (tn) #f (zero? +3))
;; decimal (explicit)
(assert-eq? (tn) #t (zero? #d0))
(assert-eq? (tn) #t (zero? #d-0))
(assert-eq? (tn) #t (zero? #d+0))
(assert-eq? (tn) #f (zero? #d1))
(assert-eq? (tn) #f (zero? #d-1))
(assert-eq? (tn) #f (zero? #d+1))
(assert-eq? (tn) #f (zero? #d3))
(assert-eq? (tn) #f (zero? #d-3))
(assert-eq? (tn) #f (zero? #d+3))
;; hexadecimal
(assert-eq? (tn) #t (zero? #x0))
(assert-eq? (tn) #t (zero? #x-0))
(assert-eq? (tn) #t (zero? #x+0))
(assert-eq? (tn) #f (zero? #x1))
(assert-eq? (tn) #f (zero? #x-1))
(assert-eq? (tn) #f (zero? #x+1))
(assert-eq? (tn) #f (zero? #x3))
(assert-eq? (tn) #f (zero? #x-3))
(assert-eq? (tn) #f (zero? #x+3))
(assert-eq? (tn) #f (zero? #xa))
(assert-eq? (tn) #f (zero? #x-a))
(assert-eq? (tn) #f (zero? #x+a))

(assert-error (tn) (lambda () (zero? 'symbol)))
(assert-error (tn) (lambda () (zero? 'SYMBOL)))
(assert-error (tn) (lambda () (zero? #\a)))
(assert-error (tn) (lambda () (zero? #\あ)))
(assert-error (tn) (lambda () (zero? "")))
(assert-error (tn) (lambda () (zero? " ")))
(assert-error (tn) (lambda () (zero? "a")))
(assert-error (tn) (lambda () (zero? "A")))
(assert-error (tn) (lambda () (zero? "aBc12!")))
(assert-error (tn) (lambda () (zero? "あ")))
(assert-error (tn) (lambda () (zero? "あ0イう12!")))
(assert-error (tn) (lambda () (zero? +)))
(assert-error (tn) (lambda () (zero? (lambda () #t))))

;; syntactic keywords should not be appeared as operand
(if sigscheme?
    (begin
      ;; pure syntactic keyword
      (assert-error (tn) (lambda () (zero? else)))
      ;; expression keyword
      (assert-error (tn) (lambda () (zero? do)))))

(call-with-current-continuation
 (lambda (k)
   (assert-error (tn) (lambda () (zero? k)))))
(assert-error (tn) (lambda () (zero? (current-output-port))))
(assert-error (tn) (lambda () (zero? '(#t . #t))))
(assert-error (tn) (lambda () (zero? (cons #t #t))))
(assert-error (tn) (lambda () (zero? '(0 1 2))))
(assert-error (tn) (lambda () (zero? (list 0 1 2))))
(assert-error (tn) (lambda () (zero? '#())))
(assert-error (tn) (lambda () (zero? (vector))))
(assert-error (tn) (lambda () (zero? '#(0 1 2))))
(assert-error (tn) (lambda () (zero? (vector 0 1 2))))

(tn "positive?")
(assert-error (tn) (lambda () (positive? #f)))
(assert-error (tn) (lambda () (positive? #t)))
(assert-error (tn) (lambda () (positive? '())))
(if (provided? "sigscheme")
    (begin
      (assert-error (tn) (lambda () (positive? (eof))))
      (assert-error (tn) (lambda () (positive? (undef))))))

;; binary
(assert-eq? (tn) #f (positive? #b0))
(assert-eq? (tn) #f (positive? #b-0))
(assert-eq? (tn) #f (positive? #b+0))
(assert-eq? (tn) #t (positive? #b1))
(assert-eq? (tn) #f (positive? #b-1))
(assert-eq? (tn) #t (positive? #b+1))
;; octal
(assert-eq? (tn) #f (positive? #o0))
(assert-eq? (tn) #f (positive? #o-0))
(assert-eq? (tn) #f (positive? #o+0))
(assert-eq? (tn) #t (positive? #o1))
(assert-eq? (tn) #f (positive? #o-1))
(assert-eq? (tn) #t (positive? #o+1))
(assert-eq? (tn) #t (positive? #o3))
(assert-eq? (tn) #f (positive? #o-3))
(assert-eq? (tn) #t (positive? #o+3))
;; decimal (implicit)
(assert-eq? (tn) #f (positive? 0))
(assert-eq? (tn) #f (positive? -0))
(assert-eq? (tn) #f (positive? +0))
(assert-eq? (tn) #t (positive? 1))
(assert-eq? (tn) #f (positive? -1))
(assert-eq? (tn) #t (positive? +1))
(assert-eq? (tn) #t (positive? 3))
(assert-eq? (tn) #f (positive? -3))
(assert-eq? (tn) #t (positive? +3))
;; decimal (explicit)
(assert-eq? (tn) #f (positive? #d0))
(assert-eq? (tn) #f (positive? #d-0))
(assert-eq? (tn) #f (positive? #d+0))
(assert-eq? (tn) #t (positive? #d1))
(assert-eq? (tn) #f (positive? #d-1))
(assert-eq? (tn) #t (positive? #d+1))
(assert-eq? (tn) #t (positive? #d3))
(assert-eq? (tn) #f (positive? #d-3))
(assert-eq? (tn) #t (positive? #d+3))
;; hexadecimal
(assert-eq? (tn) #f (positive? #x0))
(assert-eq? (tn) #f (positive? #x-0))
(assert-eq? (tn) #f (positive? #x+0))
(assert-eq? (tn) #t (positive? #x1))
(assert-eq? (tn) #f (positive? #x-1))
(assert-eq? (tn) #t (positive? #x+1))
(assert-eq? (tn) #t (positive? #x3))
(assert-eq? (tn) #f (positive? #x-3))
(assert-eq? (tn) #t (positive? #x+3))
(assert-eq? (tn) #t (positive? #xa))
(assert-eq? (tn) #f (positive? #x-a))
(assert-eq? (tn) #t (positive? #x+a))

(assert-error (tn) (lambda () (positive? 'symbol)))
(assert-error (tn) (lambda () (positive? 'SYMBOL)))
(assert-error (tn) (lambda () (positive? #\a)))
(assert-error (tn) (lambda () (positive? #\あ)))
(assert-error (tn) (lambda () (positive? "")))
(assert-error (tn) (lambda () (positive? " ")))
(assert-error (tn) (lambda () (positive? "a")))
(assert-error (tn) (lambda () (positive? "A")))
(assert-error (tn) (lambda () (positive? "aBc12!")))
(assert-error (tn) (lambda () (positive? "あ")))
(assert-error (tn) (lambda () (positive? "あ0イう12!")))
(assert-error (tn) (lambda () (positive? +)))
(assert-error (tn) (lambda () (positive? (lambda () #t))))

;; syntactic keywords should not be appeared as operand
(if sigscheme?
    (begin
      ;; pure syntactic keyword
      (assert-error (tn) (lambda () (positive? else)))
      ;; expression keyword
      (assert-error (tn) (lambda () (positive? do)))))

(call-with-current-continuation
 (lambda (k)
   (assert-error (tn) (lambda () (positive? k)))))
(assert-error (tn) (lambda () (positive? (current-output-port))))
(assert-error (tn) (lambda () (positive? '(#t . #t))))
(assert-error (tn) (lambda () (positive? (cons #t #t))))
(assert-error (tn) (lambda () (positive? '(0 1 2))))
(assert-error (tn) (lambda () (positive? (list 0 1 2))))
(assert-error (tn) (lambda () (positive? '#())))
(assert-error (tn) (lambda () (positive? (vector))))
(assert-error (tn) (lambda () (positive? '#(0 1 2))))
(assert-error (tn) (lambda () (positive? (vector 0 1 2))))

(tn "negative?")
(assert-error (tn) (lambda () (negative? #f)))
(assert-error (tn) (lambda () (negative? #t)))
(assert-error (tn) (lambda () (negative? '())))
(if (provided? "sigscheme")
    (begin
      (assert-error (tn) (lambda () (negative? (eof))))
      (assert-error (tn) (lambda () (negative? (undef))))))

;; binary
(assert-eq? (tn) #f (negative? #b0))
(assert-eq? (tn) #f (negative? #b-0))
(assert-eq? (tn) #f (negative? #b+0))
(assert-eq? (tn) #f (negative? #b1))
(assert-eq? (tn) #t (negative? #b-1))
(assert-eq? (tn) #f (negative? #b+1))
;; octal
(assert-eq? (tn) #f (negative? #o0))
(assert-eq? (tn) #f (negative? #o-0))
(assert-eq? (tn) #f (negative? #o+0))
(assert-eq? (tn) #f (negative? #o1))
(assert-eq? (tn) #t (negative? #o-1))
(assert-eq? (tn) #f (negative? #o+1))
(assert-eq? (tn) #f (negative? #o3))
(assert-eq? (tn) #t (negative? #o-3))
(assert-eq? (tn) #f (negative? #o+3))
;; decimal (implicit)
(assert-eq? (tn) #f (negative? 0))
(assert-eq? (tn) #f (negative? -0))
(assert-eq? (tn) #f (negative? +0))
(assert-eq? (tn) #f (negative? 1))
(assert-eq? (tn) #t (negative? -1))
(assert-eq? (tn) #f (negative? +1))
(assert-eq? (tn) #f (negative? 3))
(assert-eq? (tn) #t (negative? -3))
(assert-eq? (tn) #f (negative? +3))
;; decimal (explicit)
(assert-eq? (tn) #f (negative? #d0))
(assert-eq? (tn) #f (negative? #d-0))
(assert-eq? (tn) #f (negative? #d+0))
(assert-eq? (tn) #f (negative? #d1))
(assert-eq? (tn) #t (negative? #d-1))
(assert-eq? (tn) #f (negative? #d+1))
(assert-eq? (tn) #f (negative? #d3))
(assert-eq? (tn) #t (negative? #d-3))
(assert-eq? (tn) #f (negative? #d+3))
;; hexadecimal
(assert-eq? (tn) #f (negative? #x0))
(assert-eq? (tn) #f (negative? #x-0))
(assert-eq? (tn) #f (negative? #x+0))
(assert-eq? (tn) #f (negative? #x1))
(assert-eq? (tn) #t (negative? #x-1))
(assert-eq? (tn) #f (negative? #x+1))
(assert-eq? (tn) #f (negative? #x3))
(assert-eq? (tn) #t (negative? #x-3))
(assert-eq? (tn) #f (negative? #x+3))
(assert-eq? (tn) #f (negative? #xa))
(assert-eq? (tn) #t (negative? #x-a))
(assert-eq? (tn) #f (negative? #x+a))

(assert-error (tn) (lambda () (negative? 'symbol)))
(assert-error (tn) (lambda () (negative? 'SYMBOL)))
(assert-error (tn) (lambda () (negative? #\a)))
(assert-error (tn) (lambda () (negative? #\あ)))
(assert-error (tn) (lambda () (negative? "")))
(assert-error (tn) (lambda () (negative? " ")))
(assert-error (tn) (lambda () (negative? "a")))
(assert-error (tn) (lambda () (negative? "A")))
(assert-error (tn) (lambda () (negative? "aBc12!")))
(assert-error (tn) (lambda () (negative? "あ")))
(assert-error (tn) (lambda () (negative? "あ0イう12!")))
(assert-error (tn) (lambda () (negative? +)))
(assert-error (tn) (lambda () (negative? (lambda () #t))))

;; syntactic keywords should not be appeared as operand
(if sigscheme?
    (begin
      ;; pure syntactic keyword
      (assert-error (tn) (lambda () (negative? else)))
      ;; expression keyword
      (assert-error (tn) (lambda () (negative? do)))))

(call-with-current-continuation
 (lambda (k)
   (assert-error (tn) (lambda () (negative? k)))))
(assert-error (tn) (lambda () (negative? (current-output-port))))
(assert-error (tn) (lambda () (negative? '(#t . #t))))
(assert-error (tn) (lambda () (negative? (cons #t #t))))
(assert-error (tn) (lambda () (negative? '(0 1 2))))
(assert-error (tn) (lambda () (negative? (list 0 1 2))))
(assert-error (tn) (lambda () (negative? '#())))
(assert-error (tn) (lambda () (negative? (vector))))
(assert-error (tn) (lambda () (negative? '#(0 1 2))))
(assert-error (tn) (lambda () (negative? (vector 0 1 2))))

(tn "odd?")
(assert-error (tn) (lambda () (odd? #f)))
(assert-error (tn) (lambda () (odd? #t)))
(assert-error (tn) (lambda () (odd? '())))
(if (provided? "sigscheme")
    (begin
      (assert-error (tn) (lambda () (odd? (eof))))
      (assert-error (tn) (lambda () (odd? (undef))))))

;; binary
(assert-eq? (tn) #f (odd? #b0))
(assert-eq? (tn) #f (odd? #b-0))
(assert-eq? (tn) #f (odd? #b+0))
(assert-eq? (tn) #t (odd? #b1))
(assert-eq? (tn) #t (odd? #b-1))
(assert-eq? (tn) #t (odd? #b+1))
(assert-eq? (tn) #f (odd? #b10))
(assert-eq? (tn) #f (odd? #b-10))
(assert-eq? (tn) #f (odd? #b+10))
(assert-eq? (tn) #t (odd? #b11))
(assert-eq? (tn) #t (odd? #b-11))
(assert-eq? (tn) #t (odd? #b+11))
(assert-eq? (tn) #f (odd? #b100))
(assert-eq? (tn) #f (odd? #b-100))
(assert-eq? (tn) #f (odd? #b+100))
;; octal
(assert-eq? (tn) #f (odd? #o0))
(assert-eq? (tn) #f (odd? #o-0))
(assert-eq? (tn) #f (odd? #o+0))
(assert-eq? (tn) #t (odd? #o1))
(assert-eq? (tn) #t (odd? #o-1))
(assert-eq? (tn) #t (odd? #o+1))
(assert-eq? (tn) #f (odd? #o2))
(assert-eq? (tn) #f (odd? #o-2))
(assert-eq? (tn) #f (odd? #o+2))
(assert-eq? (tn) #t (odd? #o3))
(assert-eq? (tn) #t (odd? #o-3))
(assert-eq? (tn) #t (odd? #o+3))
(assert-eq? (tn) #f (odd? #o4))
(assert-eq? (tn) #f (odd? #o-4))
(assert-eq? (tn) #f (odd? #o+4))
;; decimal (implicit)
(assert-eq? (tn) #f (odd? 0))
(assert-eq? (tn) #f (odd? -0))
(assert-eq? (tn) #f (odd? +0))
(assert-eq? (tn) #t (odd? 1))
(assert-eq? (tn) #t (odd? -1))
(assert-eq? (tn) #t (odd? +1))
(assert-eq? (tn) #f (odd? 2))
(assert-eq? (tn) #f (odd? -2))
(assert-eq? (tn) #f (odd? +2))
(assert-eq? (tn) #t (odd? 3))
(assert-eq? (tn) #t (odd? -3))
(assert-eq? (tn) #t (odd? +3))
(assert-eq? (tn) #f (odd? 4))
(assert-eq? (tn) #f (odd? -4))
(assert-eq? (tn) #f (odd? +4))
;; decimal (explicit)
(assert-eq? (tn) #f (odd? #d0))
(assert-eq? (tn) #f (odd? #d-0))
(assert-eq? (tn) #f (odd? #d+0))
(assert-eq? (tn) #t (odd? #d1))
(assert-eq? (tn) #t (odd? #d-1))
(assert-eq? (tn) #t (odd? #d+1))
(assert-eq? (tn) #f (odd? #d2))
(assert-eq? (tn) #f (odd? #d-2))
(assert-eq? (tn) #f (odd? #d+2))
(assert-eq? (tn) #t (odd? #d3))
(assert-eq? (tn) #t (odd? #d-3))
(assert-eq? (tn) #t (odd? #d+3))
(assert-eq? (tn) #f (odd? #d4))
(assert-eq? (tn) #f (odd? #d-4))
(assert-eq? (tn) #f (odd? #d+4))
;; hexadecimal
(assert-eq? (tn) #f (odd? #x0))
(assert-eq? (tn) #f (odd? #x-0))
(assert-eq? (tn) #f (odd? #x+0))
(assert-eq? (tn) #t (odd? #x1))
(assert-eq? (tn) #t (odd? #x-1))
(assert-eq? (tn) #t (odd? #x+1))
(assert-eq? (tn) #f (odd? #x2))
(assert-eq? (tn) #f (odd? #x-2))
(assert-eq? (tn) #f (odd? #x+2))
(assert-eq? (tn) #t (odd? #x3))
(assert-eq? (tn) #t (odd? #x-3))
(assert-eq? (tn) #t (odd? #x+3))
(assert-eq? (tn) #f (odd? #x4))
(assert-eq? (tn) #f (odd? #x-4))
(assert-eq? (tn) #f (odd? #x+4))
(assert-eq? (tn) #f (odd? #xa))
(assert-eq? (tn) #f (odd? #x-a))
(assert-eq? (tn) #f (odd? #x+a))
(assert-eq? (tn) #t (odd? #xb))
(assert-eq? (tn) #t (odd? #x-b))
(assert-eq? (tn) #t (odd? #x+b))

(assert-error (tn) (lambda () (odd? 'symbol)))
(assert-error (tn) (lambda () (odd? 'SYMBOL)))
(assert-error (tn) (lambda () (odd? #\a)))
(assert-error (tn) (lambda () (odd? #\あ)))
(assert-error (tn) (lambda () (odd? "")))
(assert-error (tn) (lambda () (odd? " ")))
(assert-error (tn) (lambda () (odd? "a")))
(assert-error (tn) (lambda () (odd? "A")))
(assert-error (tn) (lambda () (odd? "aBc12!")))
(assert-error (tn) (lambda () (odd? "あ")))
(assert-error (tn) (lambda () (odd? "あ0イう12!")))
(assert-error (tn) (lambda () (odd? +)))
(assert-error (tn) (lambda () (odd? (lambda () #t))))

;; syntactic keywords should not be appeared as operand
(if sigscheme?
    (begin
      ;; pure syntactic keyword
      (assert-error (tn) (lambda () (odd? else)))
      ;; expression keyword
      (assert-error (tn) (lambda () (odd? do)))))

(call-with-current-continuation
 (lambda (k)
   (assert-error (tn) (lambda () (odd? k)))))
(assert-error (tn) (lambda () (odd? (current-output-port))))
(assert-error (tn) (lambda () (odd? '(#t . #t))))
(assert-error (tn) (lambda () (odd? (cons #t #t))))
(assert-error (tn) (lambda () (odd? '(0 1 2))))
(assert-error (tn) (lambda () (odd? (list 0 1 2))))
(assert-error (tn) (lambda () (odd? '#())))
(assert-error (tn) (lambda () (odd? (vector))))
(assert-error (tn) (lambda () (odd? '#(0 1 2))))
(assert-error (tn) (lambda () (odd? (vector 0 1 2))))

(tn "even?")
(assert-error (tn) (lambda () (even? #f)))
(assert-error (tn) (lambda () (even? #t)))
(assert-error (tn) (lambda () (even? '())))
(if (provided? "sigscheme")
    (begin
      (assert-error (tn) (lambda () (even? (eof))))
      (assert-error (tn) (lambda () (even? (undef))))))

;; binary
(assert-eq? (tn) #t (even? #b0))
(assert-eq? (tn) #t (even? #b-0))
(assert-eq? (tn) #t (even? #b+0))
(assert-eq? (tn) #f (even? #b1))
(assert-eq? (tn) #f (even? #b-1))
(assert-eq? (tn) #f (even? #b+1))
(assert-eq? (tn) #t (even? #b10))
(assert-eq? (tn) #t (even? #b-10))
(assert-eq? (tn) #t (even? #b+10))
(assert-eq? (tn) #f (even? #b11))
(assert-eq? (tn) #f (even? #b-11))
(assert-eq? (tn) #f (even? #b+11))
(assert-eq? (tn) #t (even? #b100))
(assert-eq? (tn) #t (even? #b-100))
(assert-eq? (tn) #t (even? #b+100))
;; octal
(assert-eq? (tn) #t (even? #o0))
(assert-eq? (tn) #t (even? #o-0))
(assert-eq? (tn) #t (even? #o+0))
(assert-eq? (tn) #f (even? #o1))
(assert-eq? (tn) #f (even? #o-1))
(assert-eq? (tn) #f (even? #o+1))
(assert-eq? (tn) #t (even? #o2))
(assert-eq? (tn) #t (even? #o-2))
(assert-eq? (tn) #t (even? #o+2))
(assert-eq? (tn) #f (even? #o3))
(assert-eq? (tn) #f (even? #o-3))
(assert-eq? (tn) #f (even? #o+3))
(assert-eq? (tn) #t (even? #o4))
(assert-eq? (tn) #t (even? #o-4))
(assert-eq? (tn) #t (even? #o+4))
;; decimal (implicit)
(assert-eq? (tn) #t (even? 0))
(assert-eq? (tn) #t (even? -0))
(assert-eq? (tn) #t (even? +0))
(assert-eq? (tn) #f (even? 1))
(assert-eq? (tn) #f (even? -1))
(assert-eq? (tn) #f (even? +1))
(assert-eq? (tn) #t (even? 2))
(assert-eq? (tn) #t (even? -2))
(assert-eq? (tn) #t (even? +2))
(assert-eq? (tn) #f (even? 3))
(assert-eq? (tn) #f (even? -3))
(assert-eq? (tn) #f (even? +3))
(assert-eq? (tn) #t (even? 4))
(assert-eq? (tn) #t (even? -4))
(assert-eq? (tn) #t (even? +4))
;; decimal (explicit)
(assert-eq? (tn) #t (even? #d0))
(assert-eq? (tn) #t (even? #d-0))
(assert-eq? (tn) #t (even? #d+0))
(assert-eq? (tn) #f (even? #d1))
(assert-eq? (tn) #f (even? #d-1))
(assert-eq? (tn) #f (even? #d+1))
(assert-eq? (tn) #t (even? #d2))
(assert-eq? (tn) #t (even? #d-2))
(assert-eq? (tn) #t (even? #d+2))
(assert-eq? (tn) #f (even? #d3))
(assert-eq? (tn) #f (even? #d-3))
(assert-eq? (tn) #f (even? #d+3))
(assert-eq? (tn) #t (even? #d4))
(assert-eq? (tn) #t (even? #d-4))
(assert-eq? (tn) #t (even? #d+4))
;; hexadecimal
(assert-eq? (tn) #t (even? #x0))
(assert-eq? (tn) #t (even? #x-0))
(assert-eq? (tn) #t (even? #x+0))
(assert-eq? (tn) #f (even? #x1))
(assert-eq? (tn) #f (even? #x-1))
(assert-eq? (tn) #f (even? #x+1))
(assert-eq? (tn) #t (even? #x2))
(assert-eq? (tn) #t (even? #x-2))
(assert-eq? (tn) #t (even? #x+2))
(assert-eq? (tn) #f (even? #x3))
(assert-eq? (tn) #f (even? #x-3))
(assert-eq? (tn) #f (even? #x+3))
(assert-eq? (tn) #t (even? #x4))
(assert-eq? (tn) #t (even? #x-4))
(assert-eq? (tn) #t (even? #x+4))
(assert-eq? (tn) #t (even? #xa))
(assert-eq? (tn) #t (even? #x-a))
(assert-eq? (tn) #t (even? #x+a))
(assert-eq? (tn) #f (even? #xb))
(assert-eq? (tn) #f (even? #x-b))
(assert-eq? (tn) #f (even? #x+b))

(assert-error (tn) (lambda () (even? 'symbol)))
(assert-error (tn) (lambda () (even? 'SYMBOL)))
(assert-error (tn) (lambda () (even? #\a)))
(assert-error (tn) (lambda () (even? #\あ)))
(assert-error (tn) (lambda () (even? "")))
(assert-error (tn) (lambda () (even? " ")))
(assert-error (tn) (lambda () (even? "a")))
(assert-error (tn) (lambda () (even? "A")))
(assert-error (tn) (lambda () (even? "aBc12!")))
(assert-error (tn) (lambda () (even? "あ")))
(assert-error (tn) (lambda () (even? "あ0イう12!")))
(assert-error (tn) (lambda () (even? +)))
(assert-error (tn) (lambda () (even? (lambda () #t))))

;; syntactic keywords should not be appeared as operand
(if sigscheme?
    (begin
      ;; pure syntactic keyword
      (assert-error (tn) (lambda () (even? else)))
      ;; expression keyword
      (assert-error (tn) (lambda () (even? do)))))

(call-with-current-continuation
 (lambda (k)
   (assert-error (tn) (lambda () (even? k)))))
(assert-error (tn) (lambda () (even? (current-output-port))))
(assert-error (tn) (lambda () (even? '(#t . #t))))
(assert-error (tn) (lambda () (even? (cons #t #t))))
(assert-error (tn) (lambda () (even? '(0 1 2))))
(assert-error (tn) (lambda () (even? (list 0 1 2))))
(assert-error (tn) (lambda () (even? '#())))
(assert-error (tn) (lambda () (even? (vector))))
(assert-error (tn) (lambda () (even? '#(0 1 2))))
(assert-error (tn) (lambda () (even? (vector 0 1 2))))


(total-report)
