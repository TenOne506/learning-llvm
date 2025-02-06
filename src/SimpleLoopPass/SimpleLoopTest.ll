; ModuleID = '../llvm/lib/Transforms/SimpleLoopPass/SImpleLoopTest.c'
source_filename = "../llvm/lib/Transforms/SimpleLoopPass/SImpleLoopTest.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@array = dso_local local_unnamed_addr global [1024 x i32] zeroinitializer, align 16

; Function Attrs: nofree norecurse nosync nounwind memory(readwrite, argmem: none, inaccessiblemem: none) uwtable
define dso_local void @SimpleLoopTest() local_unnamed_addr #0 {
  br label %1

1:                                                ; preds = %0, %1
  %2 = phi i64 [ 0, %0 ], [ %5, %1 ]
  %3 = getelementptr inbounds [1024 x i32], ptr @array, i64 0, i64 %2
  %4 = trunc nuw nsw i64 %2 to i32
  store i32 %4, ptr %3, align 4, !tbaa !5
  %5 = add nuw nsw i64 %2, 1
  %6 = icmp eq i64 %5, 100
  br i1 %6, label %8, label %1, !llvm.loop !9

7:                                                ; preds = %8
  ret void

8:                                                ; preds = %1, %8
  %9 = phi i64 [ %14, %8 ], [ 101, %1 ]
  %10 = getelementptr inbounds [1024 x i32], ptr @array, i64 0, i64 %9
  %11 = load i32, ptr %10, align 4, !tbaa !5
  %12 = trunc nuw nsw i64 %9 to i32
  %13 = mul nsw i32 %11, %12
  store i32 %13, ptr %10, align 4, !tbaa !5
  %14 = add nuw nsw i64 %9, 1
  %15 = icmp eq i64 %14, 200
  br i1 %15, label %7, label %8, !llvm.loop !12
}

attributes #0 = { nofree norecurse nosync nounwind memory(readwrite, argmem: none, inaccessiblemem: none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 20.0.0git (https://github.com/llvm/llvm-project.git 97298853b4de70dbce9c0a140ac38e3ac179e02e)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10, !11}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!"llvm.loop.unroll.disable"}
!12 = distinct !{!12, !10, !11}
